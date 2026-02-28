#include "monitorlister.h"
#include<spawn.h>
#include<sys/wait.h>
#include<unistd.h>
#include<regex>
#include<cstring>
#include<iostream>
#include "kdeplasma/kdemonitorlisting.h"
MonitorLister::MonitorLister() {}
extern char **environ;
std::vector<std::string> MonitorLister::getMonitor(EnvVarDetector::Compositor compositor) const{
    int pipefd[2];
    if (pipe(pipefd)==-1){
        perror("pipe");
        return {};
    }
    posix_spawn_file_actions_t w_actions;
    posix_spawn_file_actions_init(&w_actions);

    posix_spawn_file_actions_adddup2(&w_actions, pipefd[1], STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&w_actions, pipefd[1], STDERR_FILENO);

    posix_spawn_file_actions_addclose(&w_actions, pipefd[0]);

    pid_t pid;
    std::vector<std::string> monitors;
    if (compositor==EnvVarDetector::Compositor::Hyprland){
        char * argv[]={
            (char *)"hyprctl",
            (char *)"monitors",
            nullptr
        };

        int status = posix_spawnp(
            &pid,
            "hyprctl",
            &w_actions,
            nullptr,
            argv,
            environ
            );
        close(pipefd[1]);
        if(status!=0){
            std::cerr<<"posix spawn failed in Hyprland: "<<strerror(status)<<"\n";
            close(pipefd[0]);
            return {};
        }

        std::string output;
        char buffer[4096];
        ssize_t count=0;
        while((count=read(pipefd[0],buffer,sizeof(buffer)))>0){
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
    // if compositor is KDE
    else if(compositor==EnvVarDetector::Compositor::KDE){
        kdemonitorlisting kdemonitorlist;
        std::vector<int> int_monitorIDs=kdemonitorlist.getMonitorListIDs();
        std::vector<std::string> str_monitorIDs;
        for(const auto& monitor:int_monitorIDs){
            str_monitorIDs.push_back(std::to_string(monitor));
        }
        return str_monitorIDs;
    }
    //if compositor is X11
    else if(compositor==EnvVarDetector::Compositor::X11){
        char *argv[]{
            (char*) "xrandr",
            nullptr
        };
        int status = posix_spawnp(
            &pid,
            "xrandr",
            &w_actions,
            nullptr,
            argv,
            environ);
        close(pipefd[1]);
        if(status!=0){
            std::cerr<<"posix spawn failed in X: "<<strerror(status)<<"\n";
            close(pipefd[0]);
            return {};
        }
        std::string output;
        char buffer[4096];
        ssize_t count=0;
        while((count=read(pipefd[0],buffer,sizeof(buffer)))>0){
            output.append(buffer,count);
        }
        close(pipefd[0]);
        waitpid(pid,nullptr,0);

        std::regex connectedMonitor(R"((\S+?)(?=\sconnected))");
        auto begin =std::sregex_iterator(output.begin(), output.end(), connectedMonitor);
        auto end = std::sregex_iterator();
        for(auto it=begin;it!=end;++it){
            monitors.push_back((*it)[1].str());
        }
        // for(const auto &monitor:monitors){
        //     std::cout<<monitor<<"\n";
        // }
    }
    posix_spawn_file_actions_destroy(&w_actions);
    return monitors;
}

