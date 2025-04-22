#ifndef SIGNCERTCONTROLLER_H
#define SIGNCERTCONTROLLER_H

#include <QObject>
#include <QThread>

#include "csrsigner.h"

namespace SignCert {

class SignCertController : public QObject
{
    Q_OBJECT
public:
    explicit SignCertController(QObject *parent = nullptr);

    Q_INVOKABLE void startSigning(
        const QString &csr,
        const QString &ca,
        const QString &caKey,
        const unsigned int days,
        const QString &outDir,
        const QString &outFileName,
        const QString &outFileExtn
        );

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
    QThread *workerThread = nullptr;
    CSRSigner *worker = nullptr;
};

}

#endif // SIGNCERTCONTROLLER_H
