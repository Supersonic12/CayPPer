#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>
#include "src/cpp/directorylister.h"
#include "src/cpp/inputtakerfromqml.h"
#include "src/cpp/imagemodel.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //now taking dir from qml
    inputTakerFromQML inputTaker;
    engine.rootContext()->setContextProperty("inputTaker",&inputTaker);
    //defining ImageModel it should take QStringList type of variable
    ImageModel imageModel;
    engine.rootContext()->setContextProperty("imageModel",&imageModel);
    QObject::connect(
        &inputTaker,
        &inputTakerFromQML::dirChanged,
        &engine,
        [&](const QString &path) {

            //For Debugging
            std::cout << "Dir changed to: "
                      << path.toStdString() << std::endl;

            std::vector<filesystem::path> files = listDirectory(std::filesystem::path(path.toStdString()));

            // process files to qfiles here
            QStringList qfiles;
            qfiles.reserve(files.size());
            for(const auto & file :files){
                const QString localPath =
                    QString::fromStdString(file.string());

                QUrl url = QUrl::fromLocalFile(localPath);

                qfiles.append(url.toString());
            }
            imageModel.setImages(qfiles);
            for(const auto &file :qfiles){
                //For Debugging
                qDebug()<<file;
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
