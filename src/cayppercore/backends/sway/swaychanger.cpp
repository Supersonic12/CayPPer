#include "swaychanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
extern char ** environ;
void swayChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors,FillMode fillMode){
    if(selectedMonitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    std::string str_mode;
    int status,wstatus;
    pid_t pid;
    if(auto mapped=mapToSway(fillMode)){
        str_mode=fromSwayModetoString(*mapped);
    }
    for(auto &monitor:selectedMonitors){
        std::string argument="-o " + monitor + " -i '"+path.string() + "' -m" +str_mode;
        char * argv[]={
            (char*) "swaybg",
            const_cast<char*>(argument.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "swaybg",
            nullptr,
            nullptr,
            argv,
            environ
            );
        if(status!=0){
            throw std::runtime_error(std::string("ERROR: swaybg couldn't called: ")+strerror(status));
        }
        waitpid(pid,&wstatus,0);
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string ("ERROR: waitpit failed in swaychanger::setWallpaper"));
        }
    }
}
std::vector<FillMode> swayChanger::supportedModes() const{
    return{
        FillMode::Center,
        FillMode::Fill,
        FillMode::Fit,
        FillMode::Stretch,
        FillMode::Tile
    };
}
