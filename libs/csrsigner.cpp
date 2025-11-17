#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

#include <QEventLoop>
#include <QFile>
#include <QScopedPointer>
#include <QTextStream>

#include <botan/asn1_obj.h>
#include <botan/auto_rng.h>
#include <botan/bigint.h>
#include <botan/data_src.h>
#include <botan/exceptn.h>
#include <botan/pkcs8.h>
#include <botan/system_rng.h>

#include "csrsigner.hpp"

using namespace std;
using namespace Botan;
using namespace SignCert;

CSRSigner::CSRSigner(QObject *parent)
    : QObject{parent}
{
#if defined(BOTAN_HAS_SYSTEM_RNG)
    rng.reset(new System_RNG);
#else
    rng.reset(new AutoSeeded_RNG);
#endif
}

void CSRSigner::setParams(const QString &csrFile,
                          const QString &caFile,
                          const QString &caKeyFile,
                          const unsigned int days,
                          const QString &outDir,
                          const QString &outFileName,
                          const bool outFileType)
{
    this->csrFile = csrFile;
    this->caFile = caFile;
    this->caKeyFile = caKeyFile;
    this->days = days;
    this->outDir = outDir;
    this->outFileName = outFileName;
    this->outFileType = outFileType;
}

void CSRSigner::sign()
{
    unique_ptr<PKCS10_Request> csr = load_csr();
    unique_ptr<X509_Certificate> ca = load_ca();
    unique_ptr<Private_Key> ca_key = load_ca_key();

    auto now = chrono::system_clock::now();

    if (!csr || !ca || !ca_key) emit finished();

    if (!ca->is_CA_cert() || ca->not_after().to_std_timepoint() < now)
    {
        emit error("CA Certificate Invalid");
        emit finished();
    }

    unique_ptr<X509_CA> ca_item = make_ca(*ca, *ca_key);

    if (!ca_item) emit finished();

    X509_Time not_before(now);
    X509_Time not_after(now + chrono::days(days));

    BigInt serial;
    serial.randomize(*rng, 120, false);
    serial.set_sign(BigInt::Positive);

    X509_Certificate out = ca_item->sign_request(*csr, *rng, serial, not_before, not_after);

    auto fileDeleter = [](QFile *f) {
        if (f->isOpen()) f->close();
        delete f;
    };

    if (outFileType)
    {
        unique_ptr<QFile, decltype(fileDeleter)> out_file(new QFile(outDir + outFileName + ".crt"), fileDeleter);

        if (!out_file->open(QIODevice::WriteOnly | QIODevice::Text,
                            QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup
                                | QFileDevice::ReadOther)) {
            emit error("Error opening out file for saving");
        } else {
            QTextStream out_stream(out_file.get());

            out_stream << QString::fromStdString(out.PEM_encode());
            out_stream.flush();

            if (out_stream.status() != QTextStream::Ok)
            {
                emit error("Error Saving PEM Format Certificate");
            }
            else
            {
                emit info("PEM Format Certificate Saved");
            }
        }
    }
    else
    {
        unique_ptr<QFile, decltype(fileDeleter)> out_file(new QFile(outDir + outFileName + ".der"), fileDeleter);

        if (!out_file->open(QIODevice::WriteOnly,
                            QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup
                                | QFileDevice::ReadOther)) {
            emit error("Error opening out file for saving");
        } else {
            vector<uint8_t> data = out.BER_encode();

            if (out_file->write(reinterpret_cast<const char *>(data.data())) == -1)
            {
                emit error("Error Saving DER Format Certificate");
            }
            else
            {
                emit info("DER Format Certificate Saved");
            }
        }
    }

    emit finished();
}

unique_ptr<PKCS10_Request> CSRSigner::load_csr()
{
    try
    {
        return make_unique<PKCS10_Request>(csrFile.toStdString());
    }
    catch (const Decoding_Error &e)
    {
        emit error("CSR Decoding Error");
    }
    catch (exception &e)
    {
        emit error("CSR Loading Failed");
    }

    return nullptr;
}

unique_ptr<X509_Certificate> CSRSigner::load_ca()
{
    try
    {
        return make_unique<X509_Certificate>(caFile.toStdString());
    }
    catch (const Decoding_Error &e)
    {
        emit error("CA Cert Decoding Error");
    }
    catch (exception &e)
    {
        emit error("CA Cert Loading Failed");
    }

    return nullptr;
}

unique_ptr<Private_Key> CSRSigner::load_ca_key()
{
    try {
        DataSource_Stream key_stream(caKeyFile.toStdString(), true);
        return PKCS8::load_key(key_stream, [this]() { return this->fetch_password(); });
    } catch (const Decoding_Error &e) {
        emit error("CA Key Decoding Error");
    } catch (exception &e) {
        emit error("CA Key Loading Failed");
    }

    return nullptr;
}

unique_ptr<X509_CA> CSRSigner::make_ca(const X509_Certificate &ca, const Private_Key &ca_key)
{
    const OID sig_oid = ca.signature_algorithm().oid();
    auto it = oidToHashAlgoMap.find(sig_oid);

    if (it != oidToHashAlgoMap.end())
        return make_unique<X509_CA>(ca, ca_key, it->second, *rng);

    emit error("Unsupported Signing Algorithm");
    return nullptr;
}

const string CSRSigner::fetch_password()
{
    emit needPassword();

    QEventLoop loop;
    connect(this, &CSRSigner::passwordReady, &loop, &QEventLoop::quit);
    loop.exec();

    string pwd = password.toStdString();
    password.clear();

    return pwd;
}

void CSRSigner::providePassword(const QString &password)
{
    this->password = password;
    emit passwordReady();
}
