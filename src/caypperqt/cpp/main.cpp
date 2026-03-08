#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QPalette>
#include <QWindow>
#include <QQuickStyle>
#include "controller.h"
using namespace std;
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //set icon
    QIcon icon = QIcon(":/qt/qml/CayPPer/qml/icons/caypper.png");
    app.setWindowIcon(icon);
    //app style selected like this because of qqc2 styles folder dialog errors. can be changed to any wanted style.
    QQuickStyle::setStyle("Fusion");
    //for config files place
    QCoreApplication::setOrganizationName("caypper");
    QCoreApplication::setApplicationName("caypper");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    //god class for everything
    Controller controller;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller",&controller);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CayPPer","Main");
    return app.exec();
}
