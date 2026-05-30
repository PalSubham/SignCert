// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>
#include <memory>
#include <vector>

#include <QEventLoop>
#include <QFile>

#include <botan/asn1_obj.h>
#include <botan/bigint.h>
#include <botan/data_src.h>
#include <botan/exceptn.h>
#include <botan/pkcs8.h>

#ifdef BOTAN_HAS_SYSTEM_RNG
    #include <botan/system_rng.h>
    #define RNG_CLASS System_RNG
#else
    #include <botan/auto_rng.h>
    #define RNG_CLASS AutoSeeded_RNG
#endif

#include "csrsigner.hpp"

using namespace std;
using namespace Botan;

namespace SignCert
{
    CSRSigner::CSRSigner(QObject *parent)
        : QObject{parent}
    {
        rng.reset(new RNG_CLASS);
    }

    void CSRSigner::sign(
        const QString &csrFile,
        const QString &caFile,
        const QString &caKeyFile,
        const qint64 days,
        const QString &outDir,
        const QString &outFileName,
        const Types::OutputFormat outFileType
    )
    {
        unique_ptr<PKCS10_Request> csr = load_csr(csrFile);
        unique_ptr<X509_Certificate> ca = load_ca(caFile);
        unique_ptr<Private_Key> ca_key = load_ca_key(caKeyFile);

        if (!csr || !ca || !ca_key)
        {
            emit finished();
            return;
        }

        auto now = chrono::system_clock::now();

        if (!ca->is_CA_cert() || ca->not_after().to_std_timepoint() < now)
        {
            emit error("CA Certificate Invalid");
            emit finished();
            return;
        }

        unique_ptr<X509_CA> ca_item = make_ca(*ca, *ca_key);

        if (!ca_item)
        {
            emit finished();
            return;
        }

        X509_Time not_before(now);
        X509_Time not_after(now + chrono::days(days));

        BigInt serial;
        serial.randomize(*rng, 120, false);
        serial.set_sign(BigInt::Positive);

        X509_Certificate out = ca_item->sign_request(*csr, *rng, serial, not_before, not_after);

        switch (outFileType)
        {
            case Types::OutputFormat::PEM:
            {
                unique_ptr<QFile, decltype(&CSRSigner::file_close)> out_file(
                    new QFile(outDir + "/" + outFileName + ".crt"),
                    &CSRSigner::file_close
                );

                if (!out_file->open(
                    QIODevice::WriteOnly | QIODevice::Text,
                    QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::ReadOther
                ))
                {
                    emit error("Error opening out file for saving");
                }
                else
                {
                    string data = out.PEM_encode();

                    if (out_file->write(data.c_str(), data.size()) == -1) {
                        emit error("Error Saving PEM Format Certificate");
                    } else {
                        emit info("PEM Format Certificate Saved");
                    }
                }

                break;
            }

            case Types::OutputFormat::DER:
            {
                unique_ptr<QFile, decltype(&CSRSigner::file_close)> out_file(
                    new QFile(outDir + "/" + outFileName + ".der"),
                    &CSRSigner::file_close
                );

                if (!out_file->open(
                    QIODevice::WriteOnly,
                    QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::ReadOther
                ))
                {
                    emit error("Error opening out file for saving");
                }
                else
                {
                    vector<uint8_t> data = out.BER_encode();

                    if (out_file->write(reinterpret_cast<const char *>(data.data()), data.size()) == -1) {
                        emit error("Error Saving DER Format Certificate");
                    } else {
                        emit info("DER Format Certificate Saved");
                    }
                }

                break;
            }
        }

        emit finished();
        return;
    }

    unique_ptr<PKCS10_Request> CSRSigner::load_csr(const QString &csrFile)
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

    unique_ptr<X509_Certificate> CSRSigner::load_ca(const QString &caFile)
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

    unique_ptr<Private_Key> CSRSigner::load_ca_key(const QString &caKeyFile)
    {
        try {
            DataSource_Stream key_stream(caKeyFile.toStdString(), true);
            return PKCS8::load_key(key_stream, [this]() { return fetch_password(); });
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
        {
            try
            {
                return make_unique<X509_CA>(ca, ca_key, it->second, *rng);
            } catch (exception &e) {
                emit error("CA generation Failed");
            }
        }
        else
        {
            emit error("Unsupported Signing Algorithm");
        }

        return nullptr;
    }

    const string CSRSigner::fetch_password()
    {
        emit needPassword();

        QEventLoop loop;
        QString password;

        connect(
            this,
            &CSRSigner::passwordReady,
            &loop,
            [&](const QString &pwd)
            {
                password = pwd;
                loop.quit();
            }
        );

        loop.exec();

        return password.toStdString();
    }

    void CSRSigner::providePassword(const QString &password)
    {
        emit passwordReady(password, QPrivateSignal());
    }

    void CSRSigner::file_close(QFile *f)
    {
        if (f)
        {
            if (f->isOpen()) f->close();

            delete f;
        }
    }
}
