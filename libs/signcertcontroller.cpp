#include "signcertcontroller.h"

using namespace SignCert;

SignCertController::SignCertController(QObject *parent)
    : QObject{parent}
{}

void SignCertController::startSigning(
    const QString &csr,
    const QString &ca,
    const QString &caKey,
    const unsigned int days,
    const QString &outDir,
    const QString &outFileName,
    const bool &outFileType
)
{
    if (workerThread) return;

    workerThread = new QThread;
    worker = new CSRSigner;

    worker->setParams(csr, ca, caKey, days, outDir, outFileName, outFileType);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &CSRSigner::sign);

    connect(worker, &CSRSigner::info, this, &SignCertController::info);
    connect(worker, &CSRSigner::warn, this, &SignCertController::warn);
    connect(worker, &CSRSigner::error, this, &SignCertController::error);
    connect(worker, &CSRSigner::debug, this, &SignCertController::debug);
    connect(worker, &CSRSigner::needPassword, this, &SignCertController::needPassword);
    connect(this, &SignCertController::passwordProvided, worker, &CSRSigner::providePassword, Qt::QueuedConnection);
    connect(worker, &CSRSigner::finished, this, [=, this]() {
        workerThread->quit();
        workerThread->wait();
        worker->deleteLater();
        workerThread->deleteLater();
        worker = nullptr;
        workerThread = nullptr;
        emit finished();
    });

    workerThread->start();
}

void SignCertController::providePassword(const QString &password)
{
    if (worker)
    {
        emit passwordProvided(password);
    }
}
