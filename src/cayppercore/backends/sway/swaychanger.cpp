#include "swaychanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
#include "../../domainExpansion/raiiguard.h"
#include <nlohmann/json.hpp>

using Json = nlohmann::json;
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
        auto search=monitorsState_.find(monitor);
        if(search!=monitorsState_.end()){
            kill(search->second.pid,SIGTERM);
            waitpid(search->second.pid,&wstatus,0);
            monitorsState_.erase(search);
        }
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
        }else{
            monitorState monState;
            monState.pid=pid;
            monState.currentWall=path;
            monState.currentFillMode=str_mode;
            monitorsState_[monitor]=monState;
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
std::vector<std::string> swayChanger::monitors() const{
    int pipefd[2];
    if(pipe(pipefd)==-1){
        throw std::runtime_error(std::string("ERROR: pipe creation failed"));
    }
    pid_t pid;
    int status,wstatus;
    posix_spawn_file_actions_t sway_actions;
    posix_spawn_file_actions_init(&sway_actions);
    posix_spawn_file_actions_adddup2(&sway_actions,pipefd[1],STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&sway_actions,pipefd[1],STDERR_FILENO);
    posix_spawn_file_actions_addclose(&sway_actions,pipefd[0]);
    RAIIGuard write_guard(pipefd[1]);
    RAIIGuard read_guard(pipefd[0]);
    char* argv[]={
        (char*)"swaymsg",
        (char*)"-t",
        (char*)"get_outputs"
    };
    status = posix_spawnp(
        &pid,
        "swaymsg",
        &sway_actions,
        nullptr,
        argv,
        environ
        );
    write_guard.release();
    posix_spawn_file_actions_destroy(&sway_actions);
    if(status!=0){
        throw std::runtime_error(std::string("ERROR: swaymsg failed with code: ")+strerror(status));
    }
    std::string raw_json;
    char buffer[4096];
    ssize_t count=0;
    while(true){
        count=read(pipefd[0],buffer,sizeof(buffer));
        if(count==0){
            break;
        }
        if(count==-1){
            if(errno==EINTR) continue;
            throw std::runtime_error(std::string("ERROR: reading from pipe failed"));
        }
        raw_json.append(buffer,count);
    }
    std::vector<std::string> availableMonitors;
    auto json=Json::parse(raw_json);
    if(json.is_array()){
        for(const auto& monitor:json){
            if(monitor.contains("name")&&monitor["name"].is_string()){
                availableMonitors.push_back(monitor["name"]);
            }
        }
    }else{
        throw std::runtime_error(std::string("ERROR: json parsing from swaymsg failed"));
    }
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error(std::string("ERROR: waitpid failed while waiting for swaymsg"));
    }
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: swaymsg failed while getting monitors"));
    }
    return availableMonitors;
}
