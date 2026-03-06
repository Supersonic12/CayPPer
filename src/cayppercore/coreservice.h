#ifndef CORESERVICE_H
#define CORESERVICE_H

#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>

#include "envvardetector.h"
#include "domainExpansion/fillmode.h"
#include "directorylister.h"
#include "IChanger.h"
class coreService
{
public:
    coreService();
    ~coreService();
    bool isWayland() const;
    EnvVarDetector::Compositor compositor() const;
    std::vector<std::string> monitors() const;
    std::vector<FillMode> supportedModes() const;
    const std::vector<std::filesystem::path> listDirectory(std::filesystem::path);
    void setWallpaper(const std::filesystem::path& wallPath,std::vector<std::string>& selectedMonitors,FillMode fillmode);
    void setDirectoryChangeCallBack(std::function<void()> callback);
    void stopWatching();
    void startWatching(const std::filesystem::path& path);

private:
    EnvVarDetector envvardetector_;
    directoryLister directorylister_;
    bool isWayland_;
    EnvVarDetector::Compositor compositor_;
    void notifyDirectoryChanged();
    std::function<void()> directoryChangedCallback_;
    void watchLoop();
    std::filesystem::path watchedPath_;
    int inotifyFD_{-1};
    int watchDescriptor_{-1};
    std::thread watcherThread_;
    std::atomic<bool> running_{false};

    //polymorphic pointer for linking backends to core service
    std::unique_ptr<IBackend> backend_;
};

#endif // CORESERVICE_H
