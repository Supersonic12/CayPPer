#include "xfcechanger.h"

void XFCEChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> monitors,FillMode fillMode){
    if(monitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    std::string str_mode;
    if(auto mapped=mapToXFCE(fillMode)){
        str_mode=fromXFCEModetoString(*mapped);
    }
    else{
        std::cerr<<"Unsupported XFCE fillMode!\n";
        return;
    }

    for(auto &monitor:monitors){
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+path.string()+"'").c_str())==0){}
        else{
            std::cerr<<"Couldn't set wallpaper on XFCE\n";
            return;
        }
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+ str_mode+"").c_str())==0){}
        else{
            std::cerr<<"Couldn't set fillmode on XFCE\n";
            return;
        }
    }
}
