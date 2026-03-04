#include "changerX.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
void XChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode){
    std::string str_mode;

    if(selectedMonitors.empty()){
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check at least one!"));
    }
    if(auto mapped=mapToXWall(fillMode)){
        str_mode=fromXWallModetoString(*mapped);
    }
    else{
        throw std::runtime_error(std::string("ERROR: mapToXWall failed\n"));
    }
    int status,wstatus;
    pid_t pid;

    for(const auto& monitor:selectedMonitors){
        char * argv[]={
            (char*)"xwallpaper",
            (char*)"--output",
            const_cast<char*>(monitor.c_str()),
            const_cast<char*>(str_mode.c_str()),
            const_cast<char*>(path.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "xwallpaper",
            nullptr,
            nullptr,
            argv,
            environ
            );
        if(status != 0){
            throw std::runtime_error
                (
                    std::string
                    (
                        "ERROR:in changer::runXWallpaper. Couldnt set wallpaper on monitor: "
                        + monitor + "\n\t"+
                        strerror(status)
                        )
                    );
        }

        if(waitpid(pid,&wstatus,0)==-1){
            throw std::runtime_error(std::string("ERROR: waitpid failed while setting wallpaper to monitor: "+monitor));
        }
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: calling xwallpaper failed"));
        }
    }
}
std::vector<FillMode> XChanger::supportedModes() const{
    return {
        FillMode::Center,
        FillMode::Focus,
        FillMode::Maximize,
        FillMode::Stretch,
        FillMode::Tile,
        FillMode::Zoom
    };
}
