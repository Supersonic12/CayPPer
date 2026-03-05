#include "changerX.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
#include <regex>
#include "../../domainExpansion/raiiguard.h"
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
std::vector<std::string> XChanger::monitors() const{
    int pipefd[2];
    if(pipe(pipefd)==-1){
        throw std::runtime_error(std::string("ERROR: pipe creation failed"));
    }
    posix_spawn_file_actions_t x_actions;
    posix_spawn_file_actions_init(&x_actions);
    posix_spawn_file_actions_adddup2(&x_actions,pipefd[1],STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&x_actions,pipefd[1],STDERR_FILENO);
    posix_spawn_file_actions_addclose(&x_actions,pipefd[0]);
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
        &x_actions,
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
