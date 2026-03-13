#include "swaychanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
#include "../../cayppercore/domainExpansion/raiiguard.h"
#include <nlohmann/json.hpp>
#include <fstream>

using Json = nlohmann::json;
extern char ** environ;
void swayChanger::setWallpaper(stateOfMons *newState){
    if(newState->name.empty()){
        throw std::runtime_error(std::string("Warning!: No Monitor Checked, Check at least one!"));
    }
    std::string str_mode;
    int status,wstatus;
    pid_t pid;
    if(auto mapped=mapToSway(newState->fillMode)){
        str_mode=fromSwayModetoString(*mapped);
    }else{
        throw std::runtime_error(std::string("ERROR: mapToSway failed, Unsupported fill mode"));
    }

    for(auto &monitor:newState->name){
        auto search=monitorsState_.find(monitor);
        if(search!=monitorsState_.end()){
            pid_t old_pid=search->second.pid;
            if(kill(old_pid,0)==0)
            kill(old_pid,SIGKILL);

            monitorsState_.erase(search);
        }
        posix_spawnattr_t attr;
        posix_spawnattr_init(&attr);
        posix_spawnattr_setflags(&attr,POSIX_SPAWN_SETPGROUP);
        posix_spawnattr_setpgroup(&attr,0);
        char * argv[]={
            (char*) "swaybg",
            (char*)"-o",
            const_cast<char*>(monitor.c_str()),
            (char*)"-i",
            const_cast<char*>(newState->wallPath.c_str()),
            (char*)"-m",
            const_cast<char*>(str_mode.c_str()),
            nullptr
        };
        status=posix_spawnp(
            &pid,
            "swaybg",
            nullptr,
            &attr,
            argv,
            environ
            );
        posix_spawnattr_destroy(&attr);
        if(status!=0){
            throw std::runtime_error(std::string("ERROR: swaybg couldn't called: ")+strerror(status));
        }else{
            monitorState monState;
            monState.pid=pid;
            monState.currentWall=newState->wallPath;
            monState.currentFillMode=str_mode;
            monitorsState_[monitor]=monState;
        }
        recordProcesses(monitorsState_);
        /*for(const auto & state: monitorsState_){
      std::cout<<"Current swaybg list: "<<state.first<<": \n"<<state.second.currentFillMode<<"\n"<<
      state.second.currentWall<<"\n"<<state.second.pid<<"\n";
    }*/

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
        (char*)"get_outputs",
        (char*)"-r",
        nullptr
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
swayChanger::swayChanger(){
    try{
        reacquireProcesses();
    }catch(std::runtime_error &e){
        std::cerr<<"ERROR: Failed to reacquire old swaybg instances:" << e.what();
    }
}
swayChanger::~swayChanger(){
    try{
        recordProcesses(monitorsState_);
    }catch(std::runtime_error &e){
        std::cerr<<"ERROR: Failed to save ongoing swaybg instances:" << e.what();
    }
}
void swayChanger::recordProcesses(std::unordered_map<std::string,monitorState>& map){
    std::filesystem::create_directories("/tmp/caypper");
    Json j_processList=Json::array();
    std::ofstream output_file("/tmp/caypper/processList.json");
    if(!output_file.is_open()){
        throw std::runtime_error("ERROR: failed to open json file");
    }else{
        for(auto const & state: map){
            Json j_processEntry;
            j_processEntry["processInfo"]=
                {
                    {"name",state.first},
                    {"pid",state.second.pid},
                    {"currentWall",state.second.currentWall},
                    {"currentFillMode",state.second.currentFillMode}
                };
            j_processList.push_back(j_processEntry);
        }
        output_file<<std::setw(4)<<j_processList<<std::endl;
        output_file.close();
    }
}
void swayChanger::reacquireProcesses(){
    Json j_processList;
    std::ifstream inputFile("/tmp/caypper/processList.json");
    if(!inputFile.is_open()){
        throw std::runtime_error(std::string("ERROR: failed to open json file to reacquire old processes"));
    }
    try{
        inputFile>>j_processList;
    }catch(Json::parse_error &e){
        throw std::runtime_error("ERROR: failed to parse processList from json file");
    }
    monitorState state;
    for(const auto & proc:j_processList){
        try{
            if(!proc.contains("processInfo")){
                throw std::runtime_error(std::string("ERROR: missing processInfo key in entry"));
            }
            if(!proc["processInfo"].contains("pid") || !proc["processInfo"].contains("name")
                || !proc["processInfo"].contains("currentWall") || !proc["processInfo"].contains("currentFillMode")){
                throw std::runtime_error(std::string("ERROR: json file missing required field"));
            }
            state.pid=proc["processInfo"]["pid"].get<int>();
            state.currentWall=proc["processInfo"]["currentWall"].get<std::string>();
            state.currentFillMode=proc["processInfo"]["currentFillMode"].get<std::string>();
            monitorsState_.insert({proc["processInfo"]["name"].get<std::string>(),state});
        }catch(Json::type_error &e){
            throw std::runtime_error(std::string("ERROR: wrong type in processInfo entry"));
        }
    }
}
extern "C"{
IBackend* createBackend(){
  return new swayChanger;
}
void destroyBackend(IBackend *b){
  delete b;
}
}
