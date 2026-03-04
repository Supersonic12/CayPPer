#include "swaychanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
extern char ** environ;
void swayChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors,FillMode fillMode){
    if(selectedMonitors.empty()){
        throw std::runtime_error(std::string("Warning!: No Monitor Checked, Check at least one!"));
    }
    std::string str_mode;
    int status,wstatus;
    pid_t pid;
    if(auto mapped=mapToSway(fillMode)){
        str_mode=fromSwayModetoString(*mapped);
    }else{
        throw std::runtime_error(std::string("ERROR: mapToSway failed, Unsupported fill mode"));
    }
    for(auto &monitor:selectedMonitors){
        char * argv[]={
            (char*) "swaybg",
            (char*)"-o",
            const_cast<char*>(monitor.c_str()),
            (char*)"-i",
            const_cast<char*>(path.c_str()),
            (char*)"-m",
            const_cast<char*>(str_mode.c_str()),
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
        if(waitpid(pid,&wstatus,0)==-1){
            throw std::runtime_error(std::string("ERROR: waitpid failed while setting wallpaper to monitor: "+monitor));
        }
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: swaybg failed while setting wallpaper"));
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
