#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include "src/cpp/directorylister.h"
#include "src/cpp/inputtakerfromqml.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //now taking dir from qml
    inputTakerFromQML inputTaker;
    engine.rootContext()->setContextProperty("inputTaker",&inputTaker);
    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::dirChanged,
        &engine,
        [&](const QString &path) {

            //For Debugging
            std::cout << "Dir changed to: "
                      << path.toStdString() << std::endl;

            auto files = listDirectory(std::filesystem::path(path.toStdString()));

            // process files here
            for(const auto & file :files){
                //For Debugging
                std::cout << file << std::endl;
            }
        });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("CayPPer", "Main");

    return app.exec();
}
