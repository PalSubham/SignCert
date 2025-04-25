#ifndef CSRSIGNER_H
#define CSRSIGNER_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

#include <botan/x509_ca.h>
#include <botan/x509cert.h>
#include <botan/pkcs10.h>
#include <botan/pkcs8.h>
#include <botan/rng.h>

#include "oidhasher.h"

using namespace std;
using namespace Botan;

namespace SignCert {

class CSRSigner : public QObject
{
    Q_OBJECT
public:
    explicit CSRSigner(QObject *parent = nullptr);

    void setParams(
        const QString &csrFile,
        const QString &caFile,
        const QString &caKeyFile,
        const unsigned int days,
        const QString &outDir,
        const QString &outFileName,
       const bool &outFileType
        );
    void sign();

public slots:
    void providePassword(const QString &password);

signals:
    void info(const QString &info);
    void warn(const QString &warn);
    void error(const QString &error);
    void needPassword();
    void finished();

    void debug(const QString &debug);

private:
    QString csrFile, caFile, caKeyFile, outDir, outFileName, password;
    bool outFileType;
    unsigned int days;

    unique_ptr<RandomNumberGenerator> rng;

    QMutex mutex;
    QWaitCondition wait;

    unique_ptr<PKCS10_Request> load_csr();
    unique_ptr<X509_Certificate> load_ca();
    unique_ptr<Private_Key> load_ca_key();

    unique_ptr<X509_CA> make_ca(const X509_Certificate &ca, const Private_Key &ca_key);

    inline static unordered_map<vector<uint8_t>, string, ByteVectorHash, ByteVectorEqual> oidToHashAlgoMap = {
        {{0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B}, "SHA-256"}, // SHA256withRSA
        {{0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C}, "SHA-384"}, // SHA384withRSA
        {{0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0D}, "SHA-512"}, // SHA512withRSA
        {{0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02}, "SHA-256"},       // ECDSA-with-SHA256
        {{0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x03}, "SHA-384"},       // ECDSA-with-SHA384
        {{0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x04}, "SHA-512"},       // ECDSA-with-SHA512
        {{0x06, 0x03, 0x2B, 0x65, 0x70}, "Pure"},                                        // Ed25519
        {{0x06, 0x03, 0x2B, 0x65, 0x71}, "Pure"},                                        // Ed448
    };
};

}

#endif // CSRSIGNER_H
