#include <QUrl>
#include "controller.h"
#include "directorylister.h"
#include "domainExpansion/fillmodeconverter.h"
#include <QDebug>
Controller::Controller(QObject *parent)
    : QObject{parent}
{
    refreshAvailableModes();
    refreshAvailableMonitors();
}

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


void Controller::callSetWallpaper(QString q_index,QString mode){
    FillMode fillmode=toFillMode(mode.toStdString());
    int index=q_index.toInt();
    QUrl url(listString.at(index));
    std::filesystem::path wallpaper=(url.toLocalFile().toStdString());
    std::vector<std::string> selectedMonitorsVector;
    for(const auto& monitor:std::as_const(selectedMonitors_)){
        selectedMonitorsVector.push_back(monitor.toStdString());
    }
    core_.setWallpaper(wallpaper,selectedMonitorsVector,fillmode);
}
void Controller::refreshAvailableModes(){
    modes_.clear();
    for(auto m: core_.supportedModes()){
        switch(m){
        case FillMode::Center:       modes_.append("Center");   break;
        case FillMode::Contain:      modes_.append("Contain");  break;
        case FillMode::Cover:        modes_.append("Cover");    break;
        case FillMode::Fill:         modes_.append("Fill");     break;
        case FillMode::Fit:          modes_.append("Fit");      break;
        case FillMode::Focus:        modes_.append("Focus");    break;
        case FillMode::Maximize:     modes_.append("Maximize"); break;
        case FillMode::Stretch:      modes_.append("Stretch");  break;
        case FillMode::Tile:         modes_.append("Tile");     break;
        case FillMode::Zoom:         modes_.append("Zoom");     break;
        }
    }
    emit modesChanged();
}
void Controller::refreshAvailableMonitors(){
    availablemonitors_.clear();
    for(const auto& monitor:core_.monitors()){
        availablemonitors_.append(QString::fromStdString(monitor));
    }
    emit monitorsChanged();
}

void Controller::refreshSelectedMonitors(const bool ischecked, const QString selectedmonitor){
    if(ischecked && !selectedMonitors_.contains(selectedmonitor)){
        selectedMonitors_.append(selectedmonitor);
    }else{
        selectedMonitors_.removeAll(selectedmonitor);
    }
    //this function isnt called yet
    //qDebug()<<selectedMonitors_<<":selectedcalled with these\n";
}

QStringList Controller::modes() const{
    return modes_;
}

QStringList Controller::connectedMonitors() const{
    //qDebug()<<availablemonitors_<<":list of available monitors\n";
    return availablemonitors_;
}
