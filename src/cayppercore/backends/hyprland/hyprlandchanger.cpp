#include "hyprlandchanger.h"
#include "../../domainExpansion/fillmodeconverter.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
extern char **environ;
void HyprChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors, FillMode fillMode){
    if(selectedMonitors.empty()){
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check At Least One!"));
    }
    auto mapped= mapToHyprland(fillMode);
    if(!mapped){
        throw std::runtime_error(std::string("ERROR: mapToHyprland failed"));
    }
    pid_t pid;
    int status;
    int wstatus;
    std::string str_mode = fromHyprModetoString(*mapped);
    std::string str_path = path.string();
    for(auto &monitor:selectedMonitors){
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
        if(waitpid(pid,&wstatus,0)==-1){
            throw std::runtime_error(std::string("ERROR: waitpid failed while setting wallpaper and fillMode to monitor: "+monitor));
        }
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: hyprctl hyprpaper failed while setting wallpaper and fillMode"));
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
