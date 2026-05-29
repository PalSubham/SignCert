// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.hpp"
#include "csrsigner.hpp"

namespace SignCert
{
    Controller::Controller(QObject *parent)
        : QObject{parent}
    {}

    void Controller::startSigning(const QString &csr,
                                  const QString &ca,
                                  const QString &caKey,
                                  const unsigned int days,
                                  const QString &outDir,
                                  const QString &outFileName,
                                  const Types::OutputFormat outFileType)
    {
        if (m_signing)
        {
            emit warn("Signing already in progress");
            return;
        }

        m_signing = true;
        emit signingChanged();

        // Only if needed
        if (!workerThread)
        {
            workerThread = new QThread;
            worker = new CSRSigner;

            worker->moveToThread(workerThread);

            // Setup
            connect(worker, &CSRSigner::info, this, &Controller::info);
            connect(worker, &CSRSigner::warn, this, &Controller::warn);
            connect(worker, &CSRSigner::error, this, &Controller::error);
            connect(worker, &CSRSigner::debug, this, &Controller::debug);
            connect(worker, &CSRSigner::needPassword, this, &Controller::needPassword);
            connect(
                this,
                &Controller::passwordProvided,
                worker,
                &CSRSigner::providePassword,
                Qt::QueuedConnection
            );
            connect(
                worker,
                &CSRSigner::finished,
                this,
                [this]()
                {
                    // Guard
                    if (!workerThread || !worker) return;

                    // Reset state
                    m_signing = false;
                    emit signingChanged();

                    emit finished();

                    workerThread->quit();
                    worker->deleteLater();
                    workerThread->deleteLater();

                    worker = nullptr;
                    workerThread = nullptr;
                }
            );

            // Start worker thread
            workerThread->start();
        }

        // Trigger actual signing
        QMetaObject::invokeMethod(
            worker,
            &CSRSigner::sign,
            Qt::QueuedConnection,
            csr,
            ca,
            caKey,
            days,
            outDir,
            outFileName,
            outFileType
        );
    }

    bool Controller::signing() const
    {
        return m_signing;
    }

    void Controller::providePassword(const QString &password)
    {
        if (worker)
        {
            emit passwordProvided(password);
        }
    }
}
