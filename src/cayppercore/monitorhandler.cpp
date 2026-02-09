#include "monitorhandler.h"
#include<spawn.h>
#include<sys/wait.h>
#include<unistd.h>
#include<regex>
#include<cstring>
#include<iostream>
MonitorHandler::MonitorHandler() {}
extern char **environ;
std::vector<std::string> MonitorHandler::getMonitor(ComposFinder::Compositor compositor) const{
    int pipefd[2];
    if (pipe(pipefd)==-1){
        perror("pipe");
        return {};
    }
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);

    posix_spawn_file_actions_adddup2(&actions, pipefd[1], STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&actions, pipefd[1], STDERR_FILENO);

    posix_spawn_file_actions_addclose(&actions, pipefd[0]);

    pid_t pid;
    std::vector<std::string> monitors;
    if (compositor==ComposFinder::Compositor::Hyprland){
        char * argv[]={
            (char *)"hyprctl",
            (char *)"monitors",
            nullptr
        };

        int status = posix_spawnp(
            &pid,
            "hyprctl",
            &actions,
            nullptr,
            argv,
            environ
            );
        posix_spawn_file_actions_destroy(&actions);
        close(pipefd[1]);
        if(status!=0){
            std::cerr<<"posix spawn failed: "<<strerror(status)<<"\n";
            close(pipefd[0]);
            return {};
        }

        std::string output;
        char buffer[4096];
        ssize_t count;
        while(count==read(pipefd[0],buffer,sizeof(buffer))){
            output.append(buffer,count);
        }

        close(pipefd[0]);
        waitpid(pid,nullptr,0);
        std::regex monitorRegex(R"(Monitor (\S+) \()");

        auto begin = std::sregex_iterator(output.begin(), output.end(), monitorRegex);
        auto end = std::sregex_iterator();
        for(auto it = begin;it!=end;++it){
            monitors.push_back((*it)[1].str());
        }

    }

    return monitors;
}
