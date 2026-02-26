#include "xfcechanger.h"
#include <iostream>
xfcechanger::xfcechanger() {}
int xfcechanger::change(std::filesystem::path path,std::vector<std::string> monitors,std::string fillmode){
    if(monitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return 1;
    }

    for(auto &monitor:monitors){
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+path.string()+"'").c_str())==0){}
        else{
            std::cerr<<"Couldn't set wallpaper on XFCE\n";
            return 1;
        }
        if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+ fillmode+"").c_str())==0){}
        else{
            std::cerr<<"Couldn't set fillmode on XFCE\n";
            return 1;
        }
    }
    return 0;
}
