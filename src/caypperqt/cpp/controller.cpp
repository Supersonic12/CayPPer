#include <QUrl>
#include "controller.h"
//#include "directorylister.h"
#include "domainExpansion/fillmodeconverter.h"
#include <QDebug>
Controller::Controller(QObject *parent)
    : QObject{parent}
{
    refreshAvailableModes();
    refreshAvailableMonitors();
    core_.setDirectoryChangeCallBack(
        [this]()
        {
            QMetaObject::invokeMethod(
                this,
                [this]
                {
                    refreshDirectoryContent(directoryPath_);
                },
                Qt::QueuedConnection);
        });
    // connect(&timerofdirectorycheck_,
    //         &QTimer::timeout,
    //         this,
    //         &Controller::checkDirectory);
    // timerofdirectorycheck_.start(300);
}
//constructor functions
//should be called by default
void Controller::refreshAvailableModes(){
    availableModes_.clear();
    for(auto m: core_.supportedModes()){
        switch(m){
        case FillMode::Center:       availableModes_.append("Center");   break;
        case FillMode::Contain:      availableModes_.append("Contain");  break;
        case FillMode::Cover:        availableModes_.append("Cover");    break;
        case FillMode::Fill:         availableModes_.append("Fill");     break;
        case FillMode::Fit:          availableModes_.append("Fit");      break;
        case FillMode::Focus:        availableModes_.append("Focus");    break;
        case FillMode::Maximize:     availableModes_.append("Maximize"); break;
        case FillMode::Stretch:      availableModes_.append("Stretch");  break;
        case FillMode::Tile:         availableModes_.append("Tile");     break;
        case FillMode::Zoom:         availableModes_.append("Zoom");     break;
        }
    }
    emit modesChanged();
}
void Controller::refreshAvailableMonitors(){
    availableMonitors_.clear();
    for(const auto& monitor:core_.monitors()){
        availableMonitors_.append(QString::fromStdString(monitor));
    }
    emit monitorsChanged();
}

void Controller::setSelectedMode(QString mode){
    selectedMode_=mode;
}
QStringList Controller::getModes() const{
    return availableModes_;
}

QStringList Controller::getConnectedMonitors() const{
    return availableMonitors_;
}




//first this will be called by qml
void Controller::setDirectoryPath(QString path){
    //emit directoryPathChanged();
    if(path==directoryPath_){
        return;
    }
    directoryPath_=path;
    //call of refreshDirectoryContent
    refreshDirectoryContent(path);

}

void Controller::refreshDirectoryContent(QString path){

    //see what is inside new path
    std::vector<std::filesystem::path> stdPath=core_.listDirectory(path.toStdString());
    //newList takes newPath's contents vector as QT list
    //IMPORTANT
    //I will need to change this to QAbstractList
    QStringList newList;
    newList.reserve(stdPath.size());

    for(const auto & file : stdPath){
        const QString localPath= QString::fromStdString(file.string());
        QUrl url = QUrl::fromLocalFile(localPath);
        newList.append(url.toString());
    }

    //if contents of directory isn't same then equalize and call signal
    if(newList!=directoryContent_){
        directoryContent_=newList;
        //emit directoryContentChanged();
        //after seeing what is inside directory path it should show in grid
        //because of that we will need to change content of imagemodel
        setImageModel(directoryContent_);
    }
}




void Controller::setWallpaper(QString q_index){
    //take string value and turn it into my fillmode enum
    FillMode fillmode=toFillMode(selectedMode_.toStdString());
    //index is accepted as QString which we will need as INT
    int index=q_index.toInt();
    //create a full link to given index of directoryContent
    QUrl url(directoryContent_.at(index));
    //turn from QUrl to std::filesystem::path
    std::filesystem::path wallpaper=(url.toLocalFile().toStdString());
    //turn selectedMonitors_ QStringList to vector<string>
    std::vector<std::string> selectedMonitorsVector;
    for(const auto& monitor:std::as_const(selectedMonitors_)){
        selectedMonitorsVector.push_back(monitor.toStdString());
    }
    //call core service
    core_.setWallpaper(wallpaper,selectedMonitorsVector,fillmode);
}

void Controller::setSelectedMonitors(const bool ischecked, const QString selectedmonitor){
    if(ischecked && !selectedMonitors_.contains(selectedmonitor)){
        selectedMonitors_.append(selectedmonitor);
    }else{
        selectedMonitors_.removeAll(selectedmonitor);
    }
}

void Controller::setImageModel(QStringList content){
    imagemodel_.setImages(content);
}
QAbstractListModel* Controller::getImageModel(){
    return &imagemodel_;
}


void Controller::checkDirectory(){
    if(directoryPath_.isEmpty())
        return;

    refreshDirectoryContent(directoryPath_);
}




