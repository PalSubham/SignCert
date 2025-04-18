#ifndef CSRSIGNER_H
#define CSRSIGNER_H

#include <memory>

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

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

    void setParams(
        const QString &csrFile,
        const QString &caFile,
        const QString &caKeyFile,
        const unsigned int days,
        const QString &outDir,
        const QString &outFileName,
        const QString &outFileExtn
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

private:
    QString csrFile, caFile, caKeyFile, outDir, outFileName, outFileExtn, password;
    unsigned int days;

    QMutex mutex;
    QWaitCondition wait;

    unique_ptr<PKCS10_Request> load_csr(const QString &path);
    unique_ptr<X509_Certificate> load_ca(const QString &path);
    unique_ptr<Private_Key> load_ca_key(const QString &path);
};

#endif // CSRSIGNER_H
