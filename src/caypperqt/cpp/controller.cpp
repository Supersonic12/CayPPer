#include <QUrl>
#include "controller.h"
#include "domainExpansion/fillmodeconverter.h"
#include <QDebug>
#include <QString>
#include "domainExpansion/monState.h"
Controller::Controller(QObject *parent)
    : QObject{parent} {

    refreshAvailableModes();
    refreshAvailableMonitors();
    QByteArray desktop = qgetenv("XDG_CURRENT_DESKTOP").toLower();
    if (desktop.contains("xfce") || desktop.contains("gnome") || desktop.contains("mate")) {
        qputenv("QT_QPA_PLATFORMTHEME", QByteArray("gtk3"));
    } else if (desktop.contains("kde")) {
        qputenv("QT_QPA_PLATFORMTHEME", QByteArray("kde"));
    }
    core_.setDirectoryChangeCallBack(
    [this]() {
        QMetaObject::invokeMethod(
            this,
        [this] {
            refreshDirectoryContent(directoryPath_);
        },
        Qt::QueuedConnection);
    });
    configPath_ = settings_.getValue("General/configPath", "Unknown").toString();
    defaultWallPath_ = settings_.getValue("General/defaultWallPath", "Unknown").toString();
    vimKeysToggle_ = settings_.getBoolValue("Behavior/vimKeys", false).toBool();

    if (!defaultWallPath_.isEmpty() && defaultWallPath_ != "Unknown") {
        setDirectoryPath(defaultWallPath_);
    }
    qDebug() << defaultWallPath_;
}
//constructor functions
//should be called by default
void Controller::refreshAvailableModes() {
    availableModes_.clear();
    std::vector<std::string> vectorModes = filterFillModes(core_.supportedModes());
    for (auto const& m : vectorModes) {
        availableModes_.append(QString::fromStdString(m));
    }
    emit modesChanged();
}
void Controller::refreshAvailableMonitors() {
    availableMonitors_.clear();
    try {
        for (const auto& monitor : core_.monitors()) {
            availableMonitors_.append(QString::fromStdString(monitor));
        }
    } catch (std::runtime_error &e) {
        std::cerr << "ERROR: While getting connected monitors: " << e.what();
    }

    emit monitorsChanged();
}

void Controller::setSelectedMode(QString mode) {
    selectedMode_ = mode;
}
QStringList Controller::getModes() const {
    return availableModes_;
}

QStringList Controller::getConnectedMonitors() const {
    return availableMonitors_;
}

void Controller::setDirectoryPath(QString path) {
    QUrl url = QUrl::fromUserInput(path);
    QString localPath;

    if (url.isLocalFile()) {
        localPath = url.toLocalFile();
    } else {
        localPath = path;
    }
    if (localPath == directoryPath_) {
        return;
    }

    directoryPath_ = localPath;
    refreshDirectoryContent(localPath);

}

void Controller::refreshDirectoryContent(QString path) {

    //see what is inside new path
    std::vector<std::filesystem::path> stdPath;
    try {
        stdPath = core_.listDirectory(path.toStdString());
    } catch (std::runtime_error &e) {
        std::cerr << "ERROR: listing directory content failed: " << e.what();
    }

    //newList takes newPath's contents vector as QT list
    //IMPORTANT
    //I will need to change this to QAbstractList
    QStringList newList;
    newList.reserve(stdPath.size());

    for (const auto & file : stdPath) {
        const QString localPath = QString::fromStdString(file.string());
        QUrl url = QUrl::fromLocalFile(localPath);
        newList.append(url.toString());
    }

    //if contents of directory isn't same then equalize and call signal
    if (newList != directoryContent_) {
        directoryContent_ = newList;
        //emit directoryContentChanged();
        //after seeing what is inside directory path it should show in grid
        //because of that we will need to change content of imagemodel
        setImageModel(directoryContent_);
    }
}




void Controller::setWallpaper(QString q_index) {
    //take string value and turn it into my fillmode enum
    FillMode fillmode = toFillMode(selectedMode_.toStdString());
    //index is accepted as QString which we will need as INT
    int index = q_index.toInt();
    //create a full link to given index of directoryContent
    std::vector<std::string> selectedMonitorsVector;
    std::filesystem::path wallpaper;
    if (index >= 0 && index < directoryContent_.size()) {
        QUrl url(directoryContent_.at(index));

        //turn from QUrl to std::filesystem::path
        wallpaper = (url.toLocalFile().toStdString());
        //turn selectedMonitors_ QStringList to vector<string>

        for (const auto& monitor : std::as_const(selectedMonitors_)) {
            selectedMonitorsVector.push_back(monitor.toStdString());
        }
    } else {
        std::cerr << "Index out of boundary of directory content\n";
        return;
    }
    //call core service
    stateOfMons newState;
    newState.fillMode = fillmode;
    newState.name = selectedMonitorsVector;
    newState.wallPath = wallpaper;
    try {
        core_.setWallpaper(&newState);
    } catch (std::runtime_error &e) {
        std::cerr << "setWallpaper in Core Service failed with this cause: " << e.what() << "\n";
    }
}

void Controller::setSelectedMonitors(const bool ischecked, const QString selectedmonitor) {
    if (ischecked && !selectedMonitors_.contains(selectedmonitor)) {
        selectedMonitors_.append(selectedmonitor);
    } else {
        selectedMonitors_.removeAll(selectedmonitor);
    }
}

void Controller::setImageModel(QStringList content) {
    imagemodel_.setImages(content);
}
QAbstractListModel* Controller::getImageModel() {
    return &imagemodel_;
}


void Controller::checkDirectory() {
    if (directoryPath_.isEmpty()) {
        return;
    }

    refreshDirectoryContent(directoryPath_);
}



//settings controls
QString Controller::configPath() {
    configPath_ = settings_.getValue("General/configPath", "Unknown").toString();
    return configPath_;
}
QString Controller::defaultWallPath() {
    defaultWallPath_ = settings_.getValue("General/defaultWallPath", "Unknown").toString();
    return defaultWallPath_;
}
bool Controller::vimKeysToggle() {
    vimKeysToggle_ = settings_.getBoolValue("Behavior/vimKeys", false).toBool();
    return vimKeysToggle_;
}


void Controller::setConfigPath(QUrl path) {
    if (configPath_ == path.toLocalFile()) {
        return;
    }
    configPath_ = path.toLocalFile();
    settings_.setValue("General/configPath", configPath_);
    emit configPathChanged();
}
void Controller::setDefaultWallPath(QUrl path) {
    if (defaultWallPath_ == path.toLocalFile()) {
        return;
    }
    defaultWallPath_ = path.toLocalFile();
    settings_.setValue("General/defaultWallPath", defaultWallPath_);
    emit defaultWallPathChanged();
}
void Controller::setVimKeysToggle(bool value) {
    if (vimKeysToggle_ == value) {
        return;
    }
    vimKeysToggle_ = value;
    settings_.setValue("Behavior/vimKeys", vimKeysToggle_);
    emit vimKeysToggleChanged();
}

bool Controller::persistenceToggle() {
    persistenceToggle_ = settings_.getBoolValue("Behavior/persistence", false).toBool();
    return persistenceToggle_;
}
void Controller::setPersistenceToggle(bool value) {
    if (persistenceToggle_ == value) {
        return;
    }
    persistenceToggle_ = value;
    settings_.setValue("Behavior/persistence", value);
    emit persistenceToggleChanged();
}

