#include "changerX.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
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
        throw std::runtime_error(std::string("ERROR: mapToXWall failed\n"));
    }
    int status,wstatus;
    pid_t pid;

    for(const auto& monitor:selectedMonitors){
        std::string argument="--output '"+monitor+"' "+str_mode+ " '" +path.string()+"'";
        char * argv[]={
            (char*)"xwallpaper",
            const_cast<char*>(argument.c_str()),
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
        waitpid(pid,&wstatus,0);
        if(!WIFEXITED(wstatus)|| WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: waitpid failed in XChanger::setWallpaper"));
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
