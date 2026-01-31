#include "controller.h"
#include <QUrl>
#include "composfinder.h"
Controller::Controller(QObject *parent)
    : QObject{parent}
{}
QStringList Controller::VectorToQList(const std::vector<std::filesystem::path> stdPath){
    listString.clear();
    listString.reserve(stdPath.size());
    for(const auto & file : stdPath){
        const QString localPath= QString::fromStdString(file.string());
        QUrl url = QUrl::fromLocalFile(localPath);
        listString.append(url.toString());
    }
    return listString;
}


int Controller::setWallpaper(QString input){
    ComposFinder composfinder;
    std::filesystem::path wallpaper=(input.toStdString());
    composfinder.isWayland(wallpaper);
    return 0;
}
