#include <stdlib.h>
#include "envvardetector.h"
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>
EnvVarDetector::EnvVarDetector() {}

bool EnvVarDetector::isWayland(){
    const char* wayland= std::getenv("WAYLAND_DISPLAY");
    if(wayland){
        return true;
    }else{
        return false;
    }
}

EnvVarDetector::Compositor EnvVarDetector::getCompositor(){
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

bool EnvVarDetector::isXFCE(){
    const char* envvar=std::getenv("XDG_CURRENT_DESKTOP");
    if(!envvar){
        return false;
    }
    std::string env=envvar;
    std::transform(env.begin(),env.end(),env.begin(),::tolower);

    std::istringstream ss(env);
    std::string part;
    while(std::getline(ss,part,':')){
        if(part == "xfce") return true;
    }
    return false;
}

