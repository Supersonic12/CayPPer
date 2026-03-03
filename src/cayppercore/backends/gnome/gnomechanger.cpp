#include "gnomechanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
#include "../../domainExpansion/raiiguard.h"
extern char **environ;
void GNOMEChanger::setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors,FillMode fillMode){

}
bool GNOMEChanger::isDarkMode(){
    int pipefd[2];
    if(pipe(pipefd)==-1){
        throw std::runtime_error(std::string("Pipe creation failed.\n"));
    }
    RAIIGuard read_guard(pipefd[0]);
    RAIIGuard write_guard(pipefd[1]);
    posix_spawn_file_actions_t gnome_actions;
    posix_spawn_file_actions_init(&gnome_actions);
    posix_spawn_file_actions_adddup2(&gnome_actions,pipefd[1],STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&gnome_actions,pipefd[1], STDERR_FILENO);
    posix_spawn_file_actions_addclose(&gnome_actions,pipefd[0]);

    pid_t pid;
    char* argv[]={
        (char *) "gsettings",
        (char *) "get",
        (char *) "org.gnome.desktop.interface",
        (char *) "color-scheme",
        nullptr
    };
    int status = posix_spawnp(
        &pid,
        "gsettings",
        &gnome_actions,
        nullptr,
        argv,
        environ
        );

    posix_spawn_file_actions_destroy(&gnome_actions);
    write_guard.release();
    if(status !=0){
        throw std::runtime_error(
            std::string("posix_spawn failed")+strerror(status));
    }
    std::string output;
    char buffer[4096];
    ssize_t count=0;
    while((count=read(pipefd[0],buffer,sizeof(buffer)))>0){
        output.append(buffer,count);
    }
    int wstatus;
    waitpid(pid,&wstatus,0);
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)!=0){
        throw std::runtime_error("waitpid failed");
    }
    return output.find("dark")!=std::string::npos;
}
void GNOMEChanger::setWallpaperAll(std::filesystem::path path, FillMode fillMode){


    pid_t pid;
    std::string str_path=std::string("file://")+std::string(path);
    int status;
    if(isDarkMode()){
        char *argv[]={
            (char*)"gsettings",
            (char*)"set",
            (char*)"org.gnome.desktop.background",
            (char*)"picture-uri-dark",
            const_cast<char*>(str_path.c_str()),
            nullptr
        };
        status = posix_spawnp(
            &pid,
            "gsettings",
            nullptr,
            nullptr,
            argv,
            environ
            );
    }else{
        char *argv[]={
            (char*)"gsettings",
            (char*)"set",
            (char*)"org.gnome.desktop.background",
            (char*)"picture-uri",
            const_cast<char*>(str_path.c_str()),
            nullptr
        };
        status = posix_spawnp(
            &pid,
            "gsettings",
            nullptr,
            nullptr,
            argv,
            environ
            );
    }
    if(status!=0){
        throw std::runtime_error(
            std::string("posix spawn failed:")+strerror(status));
    }
    waitpid(pid,nullptr,0);

}
