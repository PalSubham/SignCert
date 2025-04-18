#include "signcertcontroller.h"

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
    const QString &outFileExtn
)
{
    if (workerThread) return;

    workerThread = new QThread;
    worker = new CSRSigner;

    worker->setParams(csr, ca, caKey, days, outDir, outFileName, outFileExtn);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &CSRSigner::sign);

    connect(worker, &CSRSigner::info, this, &SignCertController::info);
    connect(worker, &CSRSigner::warn, this, &SignCertController::warn);
    connect(worker, &CSRSigner::error, this, &SignCertController::error);
    connect(worker, &CSRSigner::needPassword, this, &SignCertController::needPassword);
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
