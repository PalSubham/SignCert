#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "libs/filehandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("SignCert");
    app.setApplicationName("SignCert");
    app.setApplicationDisplayName("SignCert");

    QQmlApplicationEngine engine;

    FileHandler filehandler;

    engine.rootContext()->setContextProperty("filehandler", &filehandler);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SignCert", "Main");

    return app.exec();
}
