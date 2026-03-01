#include "hyprlandchanger.h"
#include "../../domainExpansion/fillmodeconverter.h"
void HyprChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors, FillMode fillMode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    auto mapped= mapToHyprland(fillMode);
    if(!mapped){
        std::cerr<<"Unsupported Hyprland fillmode!\n";
        return;
    }

    std::string str_mode = fromHyprModetoString(*mapped);

    for(auto &monitor:selectedMonitors){
        std::string cmd="hyprctl hyprpaper wallpaper '" + monitor + "," + path.string() + "," + str_mode + "'";
        int status = std::system(cmd.c_str());
        if(status != 0){
            std::cerr<<"there is a problem in changer::runHyprland. Couldnt set wallpaper on monitor: " + monitor + "\n";
            return;
        }
    }
}
