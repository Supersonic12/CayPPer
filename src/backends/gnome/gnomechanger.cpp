#include "gnomechanger.h"
#include <spawn.h>
#include <cstring>
#include <sys/wait.h>
#include "../../cayppercore/domainExpansion/raiiguard.h"
extern char **environ;

void GNOMEChanger::setWallpaper(stateOfMons* newState){
    setWallpaperAll(newState->wallPath,newState->fillMode);
}
std::vector<std::string> GNOMEChanger::monitors() const{
    return{};
}
bool GNOMEChanger::isDarkMode(){
    int pipefd[2];
    if(pipe(pipefd)==-1){
        throw std::runtime_error(std::string("ERROR: Pipe creation failed.\n"));
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
            std::string("ERROR: gsettings get failed: ")+strerror(status));
    }
    std::string output;
    char buffer[4096];
    ssize_t count=0;
    while((count=read(pipefd[0],buffer,sizeof(buffer)))>0){
        output.append(buffer,count);
    }
    int wstatus;
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error("ERROR: Waitpid failed during getting system color-scheme");
    }

    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)!=0){
        throw std::runtime_error("ERROR: gsettings get failed");
    }
    return output.find("dark")!=std::string::npos;
}
void GNOMEChanger::setWallpaperAll(std::filesystem::path& path, FillMode fillMode){
    if(path.empty()){
        throw std::runtime_error(std::string("ERROR: GNOMEChanger::setWallpaperAll has got empty path\n"));
    }
    pid_t pid;
    std::string str_path=std::string("file://")+std::string(path);
    int status;
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
            std::string("ERROR: gsettings set failed: ")+strerror(status)+std::string("\n"));
    }
    int wstatus;
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error("ERROR: waitpid failed during settings wallpaper in dark mode") ;
    }
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: gsettings set failed"));
    }
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
    if(status!=0){
        throw std::runtime_error(
            std::string("ERROR: gsettings set failed:")+strerror(status)+std::string("\n"));
    }
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error(std::string("ERROR: waitpid failed during setting wallpaper in light mode"));
    }
    if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: gsettings set failed"));
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
        throw std::runtime_error(std::string("ERROR: Setting fillMode failed: ")+strerror(status));
    }
    if(waitpid(pid,&wstatus,0)==-1){
        throw std::runtime_error(std::string("ERROR: waitpid failed during setting fillMode\n"));
    }
    if(!WIFEXITED(wstatus)|| WEXITSTATUS(wstatus)){
        throw std::runtime_error(std::string("ERROR: gsettings set failed"));
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
extern "C" {
IBackend* createBackend(){
    return new GNOMEChanger();
}
void destroyBackend(IBackend *b){
    delete b;
}
}
