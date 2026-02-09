#include <QUrl>
#include "controller.h"
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


void Controller::callSetWallpaper(QString q_index){

    int index=q_index.toInt();
    QUrl url(listString.at(index));
    std::filesystem::path wallpaper=(url.toLocalFile().toStdString());
    //core_.setWallpaper(wallpaper,);
}
void Controller::refreshAvailableModes(){
    modes_.clear();
    for(auto m: core_.supportedModes()){
        switch(m){
        case FillMode::Center:      return modes_.append("Center");
        case FillMode::Contain:     return modes_.append("Contain");
        case FillMode::Cover:       return modes_.append("Cover");
        case FillMode::Fill:        return modes_.append("Fill");
        case FillMode::Fit:         return modes_.append("Fit");
        case FillMode::Focus:       return modes_.append("Focus");
        case FillMode::Maximize:    return modes_.append("Maximize");
        case FillMode::Stretch:     return modes_.append("Stretch");
        case FillMode::Tile:        return modes_.append("Tile");
        case FillMode::Zoom:        return modes_.append("Zoom");
        }
    }
    emit modesChanged();
}
