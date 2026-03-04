#include "hyprlandchanger.h"
#include "../../domainExpansion/fillmodeconverter.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
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
    pid_t pid;
    int status;
    int wstatus;
    std::string str_mode = fromHyprModetoString(*mapped);
    std::string str_path = path.string();
    for(auto &monitor:selectedMonitors){
        //std::string cmd="hyprctl hyprpaper wallpaper '" + monitor + "," + path.string() + "," + str_mode + "'";
        //int status = std::system(cmd.c_str());
        std::string argument="'"+monitor+","+str_path+","+str_mode+"'";
        char* argv[]{
            (char*) "hyprctl",
            (char*) "hyprpaper",
            (char*) "wallpaper",
            const_cast<char*>(argument.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "hyprctl",
            nullptr,
            nullptr,
            argv,
            environ
            );
        if(status != 0){
            throw std::runtime_error(std::string("ERROR: calling hyprpaper failed\n\t: ")+strerror(status));
        }
        waitpid(pid,&wstatus,0);
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(
                std::string(
                    "ERROR: waitpid failed when setting wallpaper in monitor:"+
                    monitor)
                );
        }
    }
}
std::vector<FillMode> HyprChanger::supportedModes() const{
    return{
        FillMode::Contain,
        FillMode::Cover,
        FillMode::Fill,
        FillMode::Tile
    };
}
