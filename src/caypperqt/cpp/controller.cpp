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
    QByteArray desktop = qgetenv("XDG_CURRENT_DESKTOP").toLower();
    if(desktop.contains("xfce") || desktop.contains("gnome") || desktop.contains("mate")){
        qputenv("QT_QPA_PLATFORMTHEME",QByteArray("gtk3"));
    }
    else if(desktop.contains("kde"))
    {
        qputenv("QT_QPA_PLATFORMTHEME",QByteArray("kde"));
    }
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
    configPath_=settings_.getValue("General/configPath","Unknown").toString();
    defaultWallPath_=settings_.getValue("General/defaultWallPath","Unknown").toString();
    vimKeysToggle_=settings_.getBoolValue("Behavior/vimKeys",false).toBool();
}
//constructor functions
//should be called by default
void Controller::refreshAvailableModes(){
    availableModes_.clear();
    for(auto m: core_.supportedModes()){
        switch(m){
        case FillMode::Center:       availableModes_.append("Center");              break;
        case FillMode::Contain:      availableModes_.append("Contain");             break;
        case FillMode::Cover:        availableModes_.append("Cover");               break;
        case FillMode::Fill:         availableModes_.append("Fill");                break;
        case FillMode::Fit:          availableModes_.append("Fit");                 break;
        case FillMode::Focus:        availableModes_.append("Focus");               break;
        case FillMode::Maximize:     availableModes_.append("Maximize");            break;
        case FillMode::Stretch:      availableModes_.append("Stretch");             break;
        case FillMode::Tile:         availableModes_.append("Tile");                break;
        case FillMode::Zoom:         availableModes_.append("Zoom");                break;
        case FillMode::Scaled:       availableModes_.append("Scaled");              break;
        case FillMode::Spanning_Screens: availableModes_.append("SpanningScreens"); break;
        case FillMode::TileHorizontally: availableModes_.append("Tile(Horizontally)");break;
        case FillMode::TileVertically:  availableModes_.append("Tile(Vertically)");  break;
        case FillMode::ScaledCropped:   availableModes_.append("Scaled(Crop)"); break;
        case FillMode::ScaledKeepAspect: availableModes_.append("Scaled(Keep Ratio)");break;
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
    QUrl url=QUrl::fromUserInput(path);
    QString localPath;

    if(url.isLocalFile()){
        localPath=url.toLocalFile();
    }
    else{
        localPath=path;
    }
    //emit directoryPathChanged();
    if(localPath==directoryPath_){
        return;
    }

    directoryPath_=localPath;
    //call of refreshDirectoryContent
    refreshDirectoryContent(localPath);

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
    try{
        core_.setWallpaper(wallpaper,selectedMonitorsVector,fillmode);
    }catch(std::runtime_error &e){
        std::cerr<<"setWallpaper in core failed with this cause:\n->"<<e.what()<<"\n";
    }
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



//settings controls
QString Controller::configPath(){
    configPath_=settings_.getValue("General/configPath","Unknown").toString();
    return configPath_;
}
QString Controller::defaultWallPath(){
    defaultWallPath_=settings_.getValue("General/defaultWallPath","Unknown").toString();
    return defaultWallPath_;
}
bool Controller::vimKeysToggle(){
    vimKeysToggle_=settings_.getBoolValue("Behavior/vimKeys",false).toBool();
    return vimKeysToggle_;
}


void Controller::setConfigPath(QUrl path){
    if(configPath_==path.toLocalFile()){
        return;
    }
    configPath_=path.toLocalFile();
    settings_.setValue("General/configPath",configPath_);
    emit configPathChanged();
}
void Controller::setDefaultWallPath(QUrl path){
    if(defaultWallPath_==path.toLocalFile()){
        return;
    }
    defaultWallPath_=path.toLocalFile();
    settings_.setValue("General/defaultWallPath",defaultWallPath_);
    emit defaultWallPathChanged();
}
void Controller::setVimKeysToggle(bool value){
    if(vimKeysToggle_==value){
        return;
    }
    vimKeysToggle_=value;
    settings_.setValue("Behavior/vimKeys",vimKeysToggle_);
    emit vimKeysToggleChanged();
}



