// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.hpp"
#include "csrsigner.hpp"
#include "filehandler.hpp"

namespace SignCert
{
    Controller::Controller(QObject *parent)
        : QObject{parent}
    {
        workerThread = new QThread(this);
        worker = new CSRSigner;

        worker->moveToThread(workerThread);

        // Setup
        connect(worker, &CSRSigner::info, this, &Controller::info);
        connect(worker, &CSRSigner::warn, this, &Controller::warn);
        connect(worker, &CSRSigner::error, this, &Controller::error);
        connect(worker, &CSRSigner::debug, this, &Controller::debug);
        connect(worker, &CSRSigner::needPassword, this, &Controller::needPassword);
        connect(this, &Controller::passwordProvided, worker, &CSRSigner::providePassword, Qt::QueuedConnection);
        connect(
            worker,
            &CSRSigner::finished,
            this,
            [this]()
            {
                // Ignore late signal
                if (m_shuttingDown) return;

                m_signing = false;
                emit signingChanged();
                emit finished();
            }
        );
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

        // Start worker thread
        workerThread->start();
    }

    void Controller::startSigning(
        const QString &csr,
        const QString &ca,
        const QString &caKey,
        const qint64 days,
        const QString &outDir,
        const QString &outFileName,
        const Types::OutputFormat outFileType
    )
    {
        if (m_shuttingDown) return;

        if (m_signing)
        {
            emit warn("Signing already in progress");
            return;
        }

        // Initial data validation
        if (!file_check(csr))
        {
            emit error("Invalid CSR file");
            return;
        }
        if (!file_check(ca))
        {
            emit error("Invalid CA cert file");
            return;
        }
        if (!file_check(caKey))
        {
            emit error("Invalid CA key file");
            return;
        }
        if (!dir_check(outDir))
        {
            emit error("Invalid output directory");
            return;
        }
        if (days <= 0)
        {
            emit error("Invalid validity value");
            return;
        }

        m_signing = true;
        emit signingChanged();

        // Trigger signing
        QMetaObject::invokeMethod(
            worker,
            &CSRSigner::sign,
            Qt::QueuedConnection,
            csr, ca, caKey, days, outDir, outFileName, outFileType
        );
    }

    bool Controller::signing() const
    {
        return m_signing;
    }

    void Controller::providePassword(const QString &password)
    {
        if (m_shuttingDown || !worker) return;

        emit passwordProvided(password);
    }

    Controller::~Controller()
    {
        m_shuttingDown = true;

        if (!workerThread) return;

        workerThread->quit();
        workerThread->wait();

        worker = nullptr;
        workerThread = nullptr;
    }
}
