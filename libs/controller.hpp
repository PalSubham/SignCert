// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QThread>

#include "csrsigner.hpp"

namespace SignCert {

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    Q_INVOKABLE void startSigning(const QString &csr,
                                  const QString &ca,
                                  const QString &caKey,
                                  const unsigned int days,
                                  const QString &outDir,
                                  const QString &outFileName,
                                  const bool outFileType);
    Q_INVOKABLE void providePassword(const QString &password);

signals:
    void info(const QString &info);
    void warn(const QString &warn);
    void error(const QString &error);
    void passwordProvided(const QString &password);
    void needPassword();
    void finished();

    void debug(const QString &debug);

private:
    QThread *workerThread = nullptr;
    CSRSigner *worker = nullptr;
};

}
