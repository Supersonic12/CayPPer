#include <stdlib.h>
#include "envvardetector.h"
#include <cstdlib>
#include <algorithm>
#include <sstream>
EnvVarDetector::EnvVarDetector() {}

bool EnvVarDetector::isWayland(){
    return std::getenv("WAYLAND_DISPLAY") != nullptr;
}
bool EnvVarDetector::isX11(){
    return std::getenv("DISPLAY") !=nullptr;
}

bool EnvVarDetector::xdgContains(const std::string &token){
    const char* xdgvar=std::getenv("XDG_CURRENT_DESKTOP");
    if(!xdgvar){
        return false;
    }
    std::string env(xdgvar);
    std::transform(env.begin(),env.end(),env.begin(),::tolower);
    std::istringstream ss(env);
    std::string part;
    std::string needle=token;
    while(std::getline(ss,part,':')){
        if(part == needle) {
            return true;
        }
    }
    return false;
}
EnvVarDetector::Compositor EnvVarDetector::getCompositor(){
    const char * compositorName;
    if(std::getenv("HYPRLAND_INSTANCE_SIGNATURE")){
        return Compositor::Hyprland;
    }
    else if(std::getenv("SWAYSOCK")){
        return Compositor::Sway;
    }else if(xdgContains("KDE")){
        return Compositor::KDE;
    }else if(xdgContains("GNOME")){
        return Compositor::GNOME;
    }else if(xdgContains("XFCE")){
        return Compositor::XFCE;
    }else if(isX11()){
        return Compositor::X11;
    }else{
        return Compositor::Unknown;
    }
};


