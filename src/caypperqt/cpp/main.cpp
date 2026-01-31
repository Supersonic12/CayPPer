#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>
#include "directorylister.h"
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

    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::dirChanged,
        &engine,
        [&](const QString &path) {
            QStringList qfiles = controller.VectorToQList(
                listDirectory(
                    std::filesystem::path(
                        path.toStdString()
                        )
                    )
                );
            imageModel.setImages(qfiles);
        });
    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::wallpaperSelected,
        &engine,
        [&](const QString &path) {
            controller.setWallpaper(path);
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
