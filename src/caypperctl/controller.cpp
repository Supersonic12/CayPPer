#include "controller.h"
using json = nlohmann::json;
Controller::Controller(QObject *parent)
    : QObject{parent}
{
}

void Controller::restoreLastWallpapers(){
    core_.restoreWallpapers();
}
void Controller::setPersistenceState(bool enabled){
    core_.setPersState(enabled);
}

