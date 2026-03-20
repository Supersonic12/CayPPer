#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QString>
#include "coreservice.h"
#include <nlohmann/json.hpp>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    void restoreLastWallpapers();
    void setPersistenceState(bool enabled);
    bool getPersistenceState();
private:
    coreService core_;
};

#endif // CONTROLLER_H
