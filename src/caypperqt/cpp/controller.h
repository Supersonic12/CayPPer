#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QStringList>
#include<QString>
#include <QObject>
#include "coreservice.h"
class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList modes READ modes NOTIFY modesChanged)
    Q_PROPERTY(QStringList connectedMonitors READ connectedMonitors NOTIFY monitorsChanged)
public:

    explicit Controller(QObject *parent = nullptr);
    //looks what is inside given path
    QStringList whatInsideDirectory(QString path);

    QStringList modes() const;

    QStringList connectedMonitors() const;

    void refreshAvailableMonitors();
    void refreshSelectedMonitors(const bool ischecked, const QString selectedmonitor);
    void callSetWallpaper(QString index,QString mode);

    Q_INVOKABLE void refreshAvailableModes();
signals:
    void modesChanged();
    void monitorsChanged();
private:
    coreService core_;
    QStringList listString;
    QStringList modes_;
    QStringList availablemonitors_;
    QStringList selectedMonitors_;
    // FillMode selectedMode;
};

#endif // CONTROLLER_H
