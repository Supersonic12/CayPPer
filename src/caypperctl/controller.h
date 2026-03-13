#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QSettings>
#include <QObject>
#include <QString>
#include "coreservice.h"
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    void restoreLastWallpapers();
    std::vector<coreService::stateOfMon> getLastStatesVector();
private:
    std::vector<coreService::stateOfMon> lastStatesVector_;
    coreService core_;
    QSettings settings_;
};

#endif // CONTROLLER_H
