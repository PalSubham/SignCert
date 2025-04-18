#include <fstream>
#include <memory>
#include <vector>

#include <QFile>
#include <QScopedPointer>

#include <botan/data_src.h>

#include "csrsigner.h"

using namespace std;
using namespace Botan;

CSRSigner::CSRSigner(QObject *parent)
    : QObject{parent}
{}

void CSRSigner::setParams(
    const QString &csrFile,
    const QString &caFile,
    const QString &caKeyFile,
    const unsigned int days,
    const QString &outDir,
    const QString &outFileName,
    const QString &outFileExtn
)
{
    this->csrFile = csrFile;
    this->caFile = caFile;
    this->caKeyFile = caKeyFile;
    this->days = days;
    this->outDir = outDir;
    this->outFileName = outFileName;
    this->outFileExtn = outFileExtn;
}

void CSRSigner::sign()
{

}

vector<uint8_t> CSRSigner::read_all(const QString &path)
{
    ifstream in(path.toStdString(), ios::binary);
    return vector<uint8_t>(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
}

unique_ptr<PKCS10_Request> CSRSigner::load_csr(const QString &path)
{
    try
    {
        return make_unique<PKCS10_Request>(path.toStdString());
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

unique_ptr<X509_Certificate> CSRSigner::load_ca(const QString &path)
{
    try
    {
        return make_unique<X509_Certificate>(path.toStdString());
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

unique_ptr<Private_Key> CSRSigner::load_ca_key(const QString &path)
{

}
