// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.hpp"

using namespace SignCert;

Controller::Controller(QObject *parent)
    : QObject{parent}
{}

void Controller::startSigning(const QString &csr,
                              const QString &ca,
                              const QString &caKey,
                              const unsigned int days,
                              const QString &outDir,
                              const QString &outFileName,
                              const bool outFileType)
{
    if (workerThread) return;

    workerThread = new QThread;
    worker = new CSRSigner;

    worker->setParams(csr, ca, caKey, days, outDir, outFileName, outFileType);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &CSRSigner::sign);

    connect(worker, &CSRSigner::info, this, &Controller::info);
    connect(worker, &CSRSigner::warn, this, &Controller::warn);
    connect(worker, &CSRSigner::error, this, &Controller::error);
    connect(worker, &CSRSigner::debug, this, &Controller::debug);
    connect(worker, &CSRSigner::needPassword, this, &Controller::needPassword);
    connect(this,
            &Controller::passwordProvided,
            worker,
            &CSRSigner::providePassword,
            Qt::QueuedConnection);
    connect(worker, &CSRSigner::finished, this, [=, this]() {
        workerThread->quit();
        worker->deleteLater();
        workerThread->deleteLater();
        worker = nullptr;
        workerThread = nullptr;
        emit finished();
    });

    workerThread->start();
}

void Controller::providePassword(const QString &password)
{
    if (worker)
    {
        emit passwordProvided(password);
    }
}
