#include "composfinder.h"
#include<stdlib.h>
#include "changer.h"
ComposFinder::ComposFinder() {}

void ComposFinder::isWayland(std::filesystem::path chosenPaper){
    Changer changer;
    const char* wayland= std::getenv("WAYLAND_DISPLAY");
    if(wayland){
        std::string compName=getCompositor();
        if(compName=="hyprland"){
            changer.runHyprland(chosenPaper);
        }else if(compName=="sway"){
            //changer.runSway(chosenPaper);
        }
    }else{
        changer.runXWallpaper(chosenPaper);
    }
}

const char* ComposFinder::getCompositor(){
    const char * compositorName;
    if(std::getenv("HYPRLAND_INSTANCE_SIGNATURE")){
        compositorName="hyprland";
    }
    else if(std::getenv("SWAYSOCK")){
        compositorName="sway";

    }else{
        compositorName="unknown";
    }
    return compositorName;
};
