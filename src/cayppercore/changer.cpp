#include "changer.h"
#include "domainExpansion/fillmodeconverter.h"
#include<iostream>

Changer::Changer() {}

int Changer::runXWallpaper(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeXWall fillmode){
    std::string str_mode=fromXWallModetoString(fillmode);
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return 1;
    }
    for(const auto& monitor:selectedMonitors){
        std::string cmd="xwallpaper --output '" + monitor +"' " + str_mode + " '" + chosenPaper.string() + "'";
        int status = std::system(cmd.c_str());
        if(status != 0){
            std::cerr<<"there is a problem in changer::runXWallpaper. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return 1;
        }
        //std::cout<<"runXwallpaper applies to this monitors:"<<monitor<<";\n";
    }
    return 0;
}
int Changer::runHyprland(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FitModeHyprland fillmode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return 1;
    }
    std::string str_mode = fromHyprModetoString(fillmode);

    for(auto &monitor:selectedMonitors){
        std::string cmd="hyprctl hyprpaper wallpaper '" + monitor + "," + chosenPaper.string() + "," + str_mode + "'";
        int status = std::system(cmd.c_str());
        if(status != 0){
            std::cerr<<"there is a problem in changer::runHyprland. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return 1;
        }
    }
    return 0;
}
int Changer::runSway(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeSway fillmode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return 1;
    }
    std::string str_mode = fromSwayModetoString(fillmode);

    for(auto &monitor:selectedMonitors){
        if(std::system(("swaybg -o " + monitor + " -i '" + chosenPaper.string() + "' -m "+ str_mode).c_str()) == 0){}
        else{
            std::cerr<<"there is a problem in changer::runSway. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return 1;
        }
    }
    return 0;
}
int Changer::runXFCE(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeXFCE fillmode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return 1;
    }
    std::string std_mode = fromXFCEModetoString(fillmode);
    for(auto &monitor:selectedMonitors){
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+chosenPaper.string()+"'").c_str())==0){}
        else{
            std::cerr<<"Couldn't set wallpaper on XFCE\n";
            return 1;
        }
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+ std_mode+"").c_str())==0){}
        else{
            std::cerr<<"Couldn't set fillmode on XFCE\n";
            return 1;
        }
    }
    return 0;
}
