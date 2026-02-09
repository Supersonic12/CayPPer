#include <stdlib.h>
#include "composfinder.h"
#include "changer.h"

ComposFinder::ComposFinder() {}

bool ComposFinder::isWayland(){
    Changer changer;
    const char* wayland= std::getenv("WAYLAND_DISPLAY");
    if(wayland){
        return true;
    }else{
        return false;
    }
}

ComposFinder::Compositor ComposFinder::getCompositor(){
    const char * compositorName;
    if(std::getenv("HYPRLAND_INSTANCE_SIGNATURE")){
        return Compositor::Hyprland;
    }
    else if(std::getenv("SWAYSOCK")){
        return Compositor::Sway;
    }else{
        return Compositor::Unknown;
    }
};


