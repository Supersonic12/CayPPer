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
            std::string("posix_spawn failed")+strerror(status)+std::string("\n"));
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
        throw std::runtime_error("Waitpid failed during getting system color-scheme.\n");
    }
    return output.find("dark")!=std::string::npos;
}
void GNOMEChanger::setWallpaperAll(std::filesystem::path path, FillMode fillMode){

    if(path.empty()){
        throw std::runtime_error(std::string("GNOMEChanger::setWallpaperAll has got empty path\n"));
    }
    pid_t pid;
    std::string str_path=std::string("file://")+std::string(path);
    int status;
    //if(isDarkMode()){
    char *argv1[]={
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
        argv1,
        environ
        );
    if(status!=0){
        throw std::runtime_error(
            std::string("posix spawn failed:")+strerror(status)+std::string("\n"));
    }
    int wstatus;
    waitpid(pid,&wstatus,0);
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: waitpid failed during setting wallpaper in dark mode\n"));
    }
    //}else{
    char *argv2[]={
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
        argv2,
        environ
        );
    //}
    if(status!=0){
        throw std::runtime_error(
            std::string("posix spawn failed:")+strerror(status)+std::string("\n"));
    }
    waitpid(pid,&wstatus,0);
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: waitpid failed during setting wallpaper in light mode\n"));
    }
    std::string str_mode;
    if(auto mapped=mapToGNOME(fillMode)){
        str_mode=fromGNOMEModetoString(*mapped);
    }
    else{
        throw std::runtime_error(std::string("ERROR: mapToGNOME function returned nullopt\n"));
    }

    char *argv3[]={
        (char *)"gsettings",
        (char *)"set",
        (char *)"org.gnome.desktop.background",
        (char *)"picture-options",
        const_cast<char *>(str_mode.c_str()),
        nullptr
    };

    status=posix_spawnp(
        &pid,
        "gsettings",
        nullptr,
        nullptr,
        argv3,
        environ
        );
    if(status!=0){
        throw std::runtime_error(std::string("Setting fillMode failed: ")+strerror(status));
    }
    waitpid(pid,&wstatus,0);
    if(!WIFEXITED(wstatus)|| WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: waitpid failed during setting fillMode\n"));
    }

}
std::vector<FillMode> GNOMEChanger::supportedModes() const{
    return{
        FillMode::Center,
        FillMode::Scaled,
        FillMode::Stretch,
        FillMode::Tile,
        FillMode::Zoom,
        FillMode::Spanning_Screens
    };
}

