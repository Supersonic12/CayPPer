#include "hyprlandchanger.h"
#include "../../cayppercore/domainExpansion/fillmodeconverter.h"
#include <spawn.h>
#include <sys/wait.h>
#include <cstring>
#include "../../cayppercore/domainExpansion/raiiguard.h"
#include <regex>
extern char** environ;
void HyprChanger::setWallpaper(stateOfMons* newState) {
    if (newState->name.empty()) {
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check At Least One!"));
    }
    auto mapped = mapToHyprland(newState->fillMode);
    if (!mapped) {
        throw std::runtime_error(std::string("ERROR: mapToHyprland failed"));
    }
    pid_t pid;
    int status;
    int wstatus;
    //KEEP THIS FOR OLD VERSIONS OF HYPRLAND
    //IT IS DROPPED FOR NEW VERSIONS
    // char* argv[]={
    //     (char*) "hyprctl",
    //     (char*) "hyprpaper",
    //     (char*) "preload",
    //     const_cast<char*>(path.c_str()),
    //     nullptr
    // };
    // status=posix_spawnp(
    //     &pid,
    //     "hyprctl",
    //     nullptr,
    //     nullptr,
    //     argv,
    //     environ);
    // if(status!=0){
    //     throw std::runtime_error(
    //         std::string("ERROR: preloading wallpaper failed with code: ")
    //         +strerror(status));
    // }
    // if(waitpid(pid,&wstatus,0)==-1){
    //     throw std::runtime_error(
    //         std::string("ERROR: waitpid failed while preloading wallpaper"));
    // }
    // if(!WIFEXITED(wstatus)||WEXITSTATUS(wstatus)){
    //     throw std::runtime_error(
    //         std::string("ERROR: preloading wallpaper failed"));
    // }
    std::string str_mode = fromHyprModetoString(*mapped);
    std::string str_path = newState->wallPath.string();
    for (auto &monitor : newState->name) {
        std::string argument = monitor + "," + str_path + "," + str_mode;
        char* argv[] {
            (char*) "hyprctl",
            (char*) "hyprpaper",
            (char*) "wallpaper",
            const_cast<char*>(argument.c_str()),
            nullptr
        };
        status = posix_spawnp(
                     &pid,
                     "hyprctl",
                     nullptr,
                     nullptr,
                     argv,
                     environ
                 );
        if (status != 0) {
            throw std::runtime_error(std::string("ERROR: calling hyprpaper failed\n\t: ") + strerror(status));
        }
        if (waitpid(pid, &wstatus, 0) == -1) {
            throw std::runtime_error(
                std::string("ERROR: waitpid failed while setting wallpaper and fillMode to monitor: " + monitor));
        }
        if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus)) {
            throw std::runtime_error(
                std::string("ERROR: hyprctl hyprpaper failed while setting wallpaper and fillMode"));
        }
    }
}
std::vector<FillMode> HyprChanger::supportedModes() const {
    return{
        FillMode::Contain,
        FillMode::Cover,
        FillMode::Fill,
        FillMode::Tile
    };
}
std::vector<std::string> HyprChanger::monitors() const {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error(std::string("ERROR: pipe creation failed"));
    }
    posix_spawn_file_actions_t hypr_actions;
    posix_spawn_file_actions_init(&hypr_actions);
    posix_spawn_file_actions_adddup2(&hypr_actions, pipefd[1], STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&hypr_actions, pipefd[1], STDERR_FILENO);
    posix_spawn_file_actions_addclose(&hypr_actions, pipefd[0]);
    RAIIGuard write_guard(pipefd[1]);
    RAIIGuard read_guard(pipefd[0]);
    pid_t pid;
    int status, wstatus;
    std::vector<std::string> availableMonitors;
    char* argv[] = {
        (char*)"hyprctl",
        (char*)"monitors",
        nullptr
    };

    status = posix_spawnp(
                 &pid,
                 "hyprctl",
                 &hypr_actions,
                 nullptr,
                 argv,
                 environ
             );
    write_guard.release();
    posix_spawn_file_actions_destroy(&hypr_actions);
    if (status != 0) {
        throw std::runtime_error(
            std::string("ERROR: hyprctl monitors failed with code:")
            + strerror(status));
    }

    std::string output;
    char buffer[4096];
    ssize_t count = 0;
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, count);
    }

    std::regex monitorRegex(R"(Monitor (\S+) \()");

    auto begin = std::sregex_iterator(output.begin(), output.end(), monitorRegex);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        availableMonitors.push_back((*it)[1].str());
    }
    return availableMonitors;
}
extern "C" {
    IBackend* createBackend() {
        return new HyprChanger;
    }
    void destroyBackend(IBackend *b) {
        delete b;
    }
}
