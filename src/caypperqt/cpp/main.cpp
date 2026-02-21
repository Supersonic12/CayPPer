#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QPalette>
#include "controller.h"
using namespace std;
int main(int argc, char *argv[])
{
    Controller *controller=new Controller;
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QCoreApplication::setOrganizationName("caypper");
    QCoreApplication::setApplicationName("caypper");
    QSettings::setDefaultFormat(QSettings::IniFormat);


    engine.rootContext()->setContextProperty("controller",controller);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CayPPer","Main");
    return app.exec();
}
