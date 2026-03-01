#include "swaychanger.h"
void swayChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors,FillMode fillMode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    std::string str_mode;
    if(auto mapped=mapToSway(fillMode)){
        str_mode=fromSwayModetoString(*mapped);
    }
    for(auto &monitor:selectedMonitors){
        if(std::system(("swaybg -o " + monitor + " -i '" + path.string() + "' -m "+ str_mode).c_str()) == 0){}
        else{
            std::cerr<<"there is a problem in changer::runSway. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return;
        }
    }
}
