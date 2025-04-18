#ifndef CSRSIGNER_H
#define CSRSIGNER_H

#include <memory>

#include <QObject>

#include <botan/x509cert.h>
#include <botan/pkcs10.h>
#include <botan/pkcs8.h>

using namespace std;
using namespace Botan;

class CSRSigner : public QObject
{
    Q_OBJECT
public:
    explicit CSRSigner(QObject *parent = nullptr);

    Q_INVOKABLE void setParams(
        const QString &csrFile,
        const QString &caFile,
        const QString &caKeyFile,
        const unsigned int days,
        const QString &outDir,
        const QString &outFileName,
        const QString &outFileExtn
    );

public slots:
    void sign();

signals:
    void info(const QString &info);
    void warn(const QString &warn);
    void error(const QString &error);
    void needPassword();
    void finished();

private:
    QString csrFile, caFile, caKeyFile, outDir, outFileName, outFileExtn;
    unsigned int days;

    unique_ptr<PKCS10_Request> load_csr(const QString &path);
    unique_ptr<X509_Certificate> load_ca(const QString &path);
    unique_ptr<Private_Key> load_ca_key(const QString &path);

    static vector<uint8_t> read_all(const QString &path);
};

#endif // CSRSIGNER_H
