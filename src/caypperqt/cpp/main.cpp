#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>



//this are qt specific modules
#include "inputtakerfromqml.h"
#include "imagemodel.h"
#include "controller.h"
using namespace std;
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //now taking dir from qml
    inputTakerFromQML inputTaker;
    //defining ImageModel this will be grids model
    ImageModel imageModel;
    Controller controller;
    //bindings
    engine.rootContext()->setContextProperty("inputTaker",&inputTaker);
    engine.rootContext()->setContextProperty("imageModel",&imageModel);
    //when directory in search field changes this changes grids model data so it changes what is shown inside
    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::dirChanged,
        &engine,
        [&](const QString &path) {
            imageModel.setImages(controller.whatInsideDirectory(path));
        });
    //when user clicked a wallpaper then this signal sets wallpaper as background
    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::wallpaperSelected,
        &engine,
        [&](const QString &index) {
            controller.callSetWallpaper(index);
        });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("CayPPer","Main");

    return app.exec();
}
