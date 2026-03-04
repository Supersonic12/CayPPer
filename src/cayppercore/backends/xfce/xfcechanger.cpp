#include "xfcechanger.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
extern char **environ;
void XFCEChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> monitors,FillMode fillMode){
    if(monitors.empty()){
        std::cerr<<"no Monitor Checked, check at least one\n";
        return;
    }
    std::string str_mode;
    if(auto mapped=mapToXFCE(fillMode)){
        str_mode=fromXFCEModetoString(*mapped);
    }
    else{
        throw std::runtime_error(std::string("ERROR:mapToXFCE return nullopt.\n"));
    }
    int status,wstatus;
    pid_t pid;

    for(auto &monitor:monitors){
        // if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+path.string()+"'").c_str())==0){}
        // else{
        //     std::cerr<<"Couldn't set wallpaper on XFCE\n";
        //     return;
        // }
        // if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+ str_mode+"").c_str())==0){}
        // else{
        //     std::cerr<<"Couldn't set fillmode on XFCE\n";
        //     return;
        // }
        std::string argument="-c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+path.string()+"'";
        char * argv[]{
            (char*)"xfconf-query",
            const_cast<char*>(argument.c_str()),
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
        waitpid(pid,&wstatus,0);
        if(!WIFEXITED(status)||WEXITSTATUS(status)){
            throw std::runtime_error(std::string("waitpid failed while setting wallpaper to monitor: "+monitor));
        }
    }

    for(auto &monitor:monitors){
        // if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/last-image -n -t string -s '"+path.string()+"'").c_str())==0){}
        // else{
        //     std::cerr<<"Couldn't set wallpaper on XFCE\n";
        //     return;
        // }
        // if(std::system(("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+ str_mode+"").c_str())==0){}
        // else{
        //     std::cerr<<"Couldn't set fillmode on XFCE\n";
        //     return;
        // }
        std::string argument="-c xfce4-desktop -p /backdrop/screen0/monitor"+monitor+"/workspace0/image-style -n -t int -s "+str_mode;
        char * argv[]{
            (char*)"xfconf-query",
            const_cast<char*>(argument.c_str()),
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
        waitpid(pid,&wstatus,0);
        if(!WIFEXITED(status)||WEXITSTATUS(status)){
            throw std::runtime_error(std::string("waitpid failed while setting fillMode to monitor: "+monitor));
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
