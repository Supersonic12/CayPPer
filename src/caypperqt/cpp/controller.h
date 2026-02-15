#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QStringList>
#include <QString>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QFileSystemWatcher>
#include "coreservice.h"
#include "imagemodel.h"
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
    //QStringList getDirectoryContent() const;
    QAbstractListModel* getImageModel();

signals:
    void modesChanged();
    void monitorsChanged();
    //void directoryContentChanged();
    void directoryPathChanged();
    void imageModelChanged();
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

    // FillMode selectedMode;
};

#endif // CONTROLLER_H
