// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QThread>
#include <QtQml/QQmlComponent>

#include "csrsigner.hpp"
#include "types.hpp"

namespace SignCert
{
    class Controller : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool signing READ signing NOTIFY signingChanged FINAL)
        QML_ELEMENT
        QML_SINGLETON

    public:
        explicit Controller(QObject *parent = nullptr);
        ~Controller() override;

        bool signing() const;

        Q_INVOKABLE void startSigning(
            const QString &csr,
            const QString &ca,
            const QString &caKey,
            const qint64 days,
            const QString &outDir,
            const QString &outFileName,
            const Types::OutputFormat outFileType
        );
        Q_INVOKABLE void providePassword(const QString &password);

    signals:
        void info(const QString &info);
        void warn(const QString &warn);
        void error(const QString &error);
        void debug(const QString &debug);
        void passwordProvided(const QString &password);
        void signingChanged();
        void needPassword();
        void finished();

    private:
        QThread *workerThread = nullptr;
        CSRSigner *worker = nullptr;
        bool m_signing = false;
        bool m_shuttingDown = false;
    };
} // namespace SignCert
