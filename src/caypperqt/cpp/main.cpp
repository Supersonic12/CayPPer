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
#include <QByteArray>
using namespace std;
int main(int argc, char* argv[]) {
    QByteArray desktop = qgetenv("XDG_CURRENT_DESKTOP").toLower();
    if (desktop.contains("xfce") || desktop.contains("gnome") || desktop.contains("mate")) {
        qputenv("QT_QPA_PLATFORMTHEME", QByteArray("gtk3"));
    } else if (desktop.contains("kde")) {
        qputenv("QT_QPA_PLATFORMTHEME", QByteArray("kde"));
    }
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

    engine.rootContext()->setContextProperty("controller", &controller);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
    []() {
        QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

    engine.loadFromModule("CayPPer", "Main");
    return app.exec();
}
