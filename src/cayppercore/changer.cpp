#include "changer.h"
#include <cstdlib>
Changer::Changer() {}

int Changer::runXWallpaper(std::filesystem::path chosenPaper){
    if(std::system(("xwallpaper --zoom \""+chosenPaper.string() + "\"").c_str())==0){
        return 0;
    }
    return 1;
}
int Changer::runHyprland(std::filesystem::path chosenPaper){
    if(std::system(("hyprctl hyprpaper wallpaper \","+chosenPaper.string()+ "\"").c_str())==0){
        return 0;
    }
    return 1;
}
int Changer::runSway(std::filesystem::path chosenPaper){
    if(
        std::system(("swaybg -o eDP-1 -i "+chosenPaper.string()+" -m fill").c_str())==0){
        return 0;
    }
    return 1;
}
