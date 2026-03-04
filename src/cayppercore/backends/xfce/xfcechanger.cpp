#include "xfcechanger.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
extern char **environ;
void XFCEChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> monitors,FillMode fillMode){
    if(monitors.empty()){
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check at least one!"));
    }
    std::string str_mode;
    if(auto mapped=mapToXFCE(fillMode)){
        str_mode=fromXFCEModetoString(*mapped);
    }
    else{
        throw std::runtime_error(std::string("ERROR: mapToXFCE failed, Unsupported fill mode"));
    }
    int status,wstatus;
    pid_t pid;
    std::string str_path=path.string();
    for(auto &monitor:monitors){
        std::string argument="/backdrop/screen0/monitor"+monitor+"/workspace0/last-image";
        char * argv[]{
            (char*)"xfconf-query",
            (char*)"-c",
            (char*)"xfce4-desktop",
            (char*)"-p",
            const_cast<char*>(argument.c_str()),
            (char*)"-n",
            (char*)"-t",
            (char*)"string",
            (char*)"-s",
            const_cast<char*>(str_path.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "xfconf-query",
            nullptr,
            nullptr,
            argv,
            environ
            );
        if(status!=0){
            throw std::runtime_error(std::string("ERROR: setting wallpaper failed with code: ")+strerror(status));
        }
        if(waitpid(pid,&wstatus,0)==-1){
            throw std::runtime_error(std::string("ERROR: waitpid failed while setting wallpaper to monitor: "+monitor));
        }
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: xfconf-query failed while setting wallpaper"));
        }
    }

    for(auto &monitor:monitors){
        std::string argument="-c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style";
        char * argv[]{
            (char*)"xfconf-query",
            (char*)"-c",
            (char*)"xfce4-desktop",
            (char*)"-p",
            const_cast<char*>(argument.c_str()),
            (char*)"-n",
            (char*)"-t",
            (char*)"int",
            (char*)"-s",
            const_cast<char*>(str_mode.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "xfconf-query",
            nullptr,
            nullptr,
            argv,
            environ
            );
        if(status!=0){
            throw std::runtime_error(std::string("ERROR: setting fillMode failed with code: ")+strerror(status));
        }

        if(waitpid(pid,&wstatus,0)==-1){
            throw std::runtime_error(std::string("ERROR: waitpid failed while setting fillMode to monitor: "+monitor));
        }
        if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
            throw std::runtime_error(std::string("ERROR: xfconf-query failed while setting fillMode"));
        }
    }
}
std::vector<FillMode> XFCEChanger::supportedModes() const{
    return{
        FillMode::Center,
        FillMode::Scaled,
        FillMode::Stretch,
        FillMode::Zoom,
        FillMode::Spanning_Screens
    };
}
