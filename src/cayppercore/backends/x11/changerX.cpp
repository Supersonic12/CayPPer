#include "changerX.h"
void XChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode){
    std::string str_mode;

    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    if(auto mapped=mapToXWall(fillMode)){
        str_mode=fromXWallModetoString(*mapped);
    }
    else{
        std::cerr<<"Unsupported XWallpaper Mode!\n";
        return;
    }
    for(const auto& monitor:selectedMonitors){
        std::string cmd="xwallpaper --output '" + monitor +"' " + str_mode + " '" + path.string() + "'";
        int status = std::system(cmd.c_str());
        if(status != 0){
            std::cerr<<"there is a problem in changer::runXWallpaper. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return;
        }
        //std::cout<<"runXwallpaper applies to this monitors:"<<monitor<<";\n";
    }
}
