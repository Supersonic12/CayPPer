#include "xfcechanger.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
#include "../../domainExpansion/raiiguard.h"
#include <regex>
extern char **environ;
void XFCEChanger::setWallpaper(std::filesystem::path& path,std::vector<std::string>& monitors,FillMode fillMode){
    if(monitors.empty()){
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check at least one!"));
    }
    std::string str_mode;
    auto mapped=mapToXFCE(fillMode);
    if(mapped){
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
        std::string argument="/backdrop/screen0/monitor"+monitor+"/workspace0/image-style";
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

std::vector<std::string> XFCEChanger::monitors() const{
    int pipefd[2];
    if(pipe(pipefd)==-1){
        throw std::runtime_error(std::string("ERROR: pipe creation failed"));
    }
    posix_spawn_file_actions_t xfce_actions;
    posix_spawn_file_actions_init(&xfce_actions);
    posix_spawn_file_actions_adddup2(&xfce_actions,pipefd[1],STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&xfce_actions,pipefd[1],STDERR_FILENO);
    posix_spawn_file_actions_addclose(&xfce_actions,pipefd[0]);
    RAIIGuard write_guard(pipefd[1]);
    RAIIGuard read_guard(pipefd[0]);
    pid_t pid;
    int status,wstatus;
    std::vector<std::string>availableMonitors;
    char *argv[]{
        (char*) "xrandr",
        nullptr
    };
    status = posix_spawnp(
        &pid,
        "xrandr",
        &xfce_actions,
        nullptr,
        argv,
        environ);
    write_guard.release();
    if(status!=0){
        throw std::runtime_error(
            std::string("ERROR: Xrandr failed with this code: ")
            +strerror(status));
    }
    std::string output;
    char buffer[4096];
    ssize_t count=0;
    while((count=read(pipefd[0],buffer,sizeof(buffer)))>0){
        output.append(buffer,count);
    }
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error(std::string("ERROR: waitpid failed while calling xrandr"));
    }
    if(!WIFEXITED(wstatus)|| WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: Xrandr failed."));
    }
    std::regex connectedMonitor(R"((\S+?)(?=\sconnected))");
    auto begin =std::sregex_iterator(output.begin(), output.end(), connectedMonitor);
    auto end = std::sregex_iterator();
    for(auto it=begin;it!=end;++it){
        availableMonitors.push_back((*it)[1].str());
    }
    return availableMonitors;
}

