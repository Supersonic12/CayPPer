#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QStringList>
#include <QObject>
#include "coreservice.h"
class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList modes READ modes NOTIFY modesChanged)
public:

    explicit Controller(QObject *parent = nullptr);
    //looks what is inside given path
    QStringList whatInsideDirectory(QString path);

    QStringList modes() const;
    void callSetWallpaper(QString index,QString mode);
    Q_INVOKABLE void refreshAvailableModes();
signals:
    void modesChanged();
private:
    coreService core_;
    QStringList listString;
    QStringList modes_;
    FillMode selectedMode;
};

#endif // CONTROLLER_H
