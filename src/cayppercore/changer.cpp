#include "changer.h"
#include "domainExpansion/fillmodeconverter.h"
#include<iostream>
#include <cstdlib>
Changer::Changer() {}

int Changer::runXWallpaper(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeXWall fillmode){
    std::string s_fillmode=fromXWallModetoString(fillmode);
    if(std::system(("xwallpaper " + s_fillmode + " '"+chosenPaper.string() + "'").c_str())==0){
        return 0;
    }
    return 1;
}
int Changer::runHyprland(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FitModeHyprland fillmode){
    std::string mode = fromHyprModetoString(fillmode);
    for(auto &monitor:selectedMonitors){
        if(std::system(("hyprctl hyprpaper wallpaper '" + monitor + "," + chosenPaper.string()  + "," + mode + "'").c_str())){}
        else{
            std::cerr<<"Couldnt set wallpaper on monitor: " + monitor + "\n";
            return 1;
        }
    }
    return 0;
}
int Changer::runSway(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeSway fillmode){
    std::string mode = fromSwayModetoString(fillmode);

    for(auto &monitor:selectedMonitors){
        if(std::system(("swaybg -o " + monitor + " -i " + chosenPaper.string() + " -m "+ mode).c_str()) == 0){}
        else{
            std::cerr<<"Couldnt set wallpaper on monitor: " + monitor + "\n";
            return 1;
        }
    }
    return 0;
}
