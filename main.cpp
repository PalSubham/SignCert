#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "libs/filehandler.h"
#include "libs/signcertcontroller.h"

using namespace SignCert;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("SignCert");
    app.setApplicationName("SignCert");
    app.setApplicationDisplayName("SignCert");

    QQmlApplicationEngine engine;

    FileHandler filehandler;
    SignCertController signCertController;

    engine.rootContext()->setContextProperty("filehandler", &filehandler);
    engine.rootContext()->setContextProperty("signcert", &signCertController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SignCert", "Main");

    int result = app.exec();

    return result;
}
