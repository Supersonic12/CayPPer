#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QStringList>
#include <QString>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QFileSystemWatcher>
#include <QUrl>
#include "coreservice.h"
#include "imagemodel.h"
#include "appsettings.h"
class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList getModes
                   READ getModes
                       NOTIFY modesChanged)
    Q_PROPERTY(QStringList getConnectedMonitors
                   READ getConnectedMonitors
                       NOTIFY monitorsChanged)
    Q_PROPERTY(QAbstractListModel* getImageModel
                   READ getImageModel
                       CONSTANT)
    Q_PROPERTY(QString configPath
               READ configPath
               NOTIFY configPathChanged)
    Q_PROPERTY(QString defaultWallPath
               READ defaultWallPath
               NOTIFY defaultWallPathChanged)
    Q_PROPERTY(bool vimKeysToggle
               READ vimKeysToggle
               NOTIFY vimKeysToggleChanged)
public:

    explicit Controller(QObject *parent = nullptr);

    void refreshAvailableMonitors();
    void refreshAvailableModes();


    Q_INVOKABLE void setSelectedMonitors(const bool ischecked, const QString selectedmonitor);
    Q_INVOKABLE void setDirectoryPath(QString path);
    Q_INVOKABLE void setWallpaper(QString index);
    Q_INVOKABLE void setSelectedMode(QString mode);
    void setImageModel(QStringList content);

    QStringList getModes() const;
    QStringList getConnectedMonitors() const;
    QAbstractListModel* getImageModel();


    QString configPath();
    Q_INVOKABLE void setConfigPath(QUrl path);
    QString defaultWallPath();
    Q_INVOKABLE void setDefaultWallPath(QUrl path);
    bool vimKeysToggle();
    Q_INVOKABLE void setVimKeysToggle(bool value);



signals:
    void modesChanged();
    void monitorsChanged();
    void directoryPathChanged();
    void imageModelChanged();
    void configPathChanged();
    void defaultWallPathChanged();
    void vimKeysToggleChanged();
    void iconColorsChanged();
private:
    void refreshDirectoryContent(QString path);
    void checkDirectory();
    // these will be set by default when launched and controller object is created.
    QStringList availableModes_;
    QStringList availableMonitors_;
    //user will set directoryPath
    QString directoryPath_;
    //inside directory Path Function this will be changed
    QStringList directoryContent_;
    //after directoryContent_ is set setImageModel should setImages in imagemodel_
    //and then getImageModel should detect Change if there is
    //and then it should show up in QML
    ImageModel imagemodel_;

    //then user selects a mode
    QString selectedMode_;
    //and a or a bunch of monitors
    QStringList selectedMonitors_;
    //and for setWallpaper create a coreservice object
    coreService core_;

    //settings object;
    appsettings settings_;

    QString configPath_;
    QString defaultWallPath_;
    bool vimKeysToggle_;

};

#endif // CONTROLLER_H
