#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "libs/controller.hpp"
#include "libs/filehandler.hpp"

using namespace SignCert;

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("SignCert");
    app.setApplicationName("SignCert");
    app.setApplicationDisplayName("SignCert");

    QQmlApplicationEngine engine;

    FileHandler filehandler;
    Controller controller;

    engine.rootContext()->setContextProperty("filehandler", &filehandler);
    engine.rootContext()->setContextProperty("controller", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SignCertQml", "Main");

    int result = app.exec();

    return result;
}
