#include "controller.h"
#include <QUrl>
#include "composfinder.h"
#include "directorylister.h"
Controller::Controller(QObject *parent)
    : QObject{parent}
{}

QStringList Controller::whatInsideDirectory(QString path){
    std::vector<std::filesystem::path> stdPath=listDirectory(path.toStdString());
    listString.clear();
    listString.reserve(stdPath.size());
    for(const auto & file : stdPath){
        const QString localPath= QString::fromStdString(file.string());
        QUrl url = QUrl::fromLocalFile(localPath);
        listString.append(url.toString());
    }
    return listString;
}


void Controller::setWallpaper(QString q_index){
    ComposFinder composfinder;

    int index=q_index.toInt();
    QUrl url(listString.at(index));
    std::filesystem::path wallpaper=(url.toLocalFile().toStdString());
    composfinder.isWayland(wallpaper);
}
