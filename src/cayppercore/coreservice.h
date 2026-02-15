#ifndef CORESERVICE_H
#define CORESERVICE_H

#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>

#include "composfinder.h"
#include "monitorlister.h"
#include "changer.h"
#include "domainExpansion/fillmode.h"
#include "directorylister.h"

class coreService
{
public:
    coreService();
    ~coreService();
    bool isWayland() const;
    ComposFinder::Compositor compositor() const;
    std::vector<std::string> monitors() const;
    std::vector<FillMode> supportedModes() const;
    const std::vector<std::filesystem::path> listDirectory(std::filesystem::path);
    void setWallpaper(const std::filesystem::path& wallPath,std::vector<std::string>& selectedMonitors,FillMode fillmode);
    void setDirectoryChangeCallBack(std::function<void()> callback);
    void stopWatching();
    void startWatching(const std::filesystem::path& path);

private:
    ComposFinder composFinder_;
    MonitorLister monitorlister_;
    directoryLister directorylister_;
    Changer changer_;
    bool isWayland_;
    ComposFinder::Compositor compositor_;
    void notifyDirectoryChanged();
    std::function<void()> directoryChangedCallback_;
    void watchLoop();
    std::filesystem::path watchedPath_;
    int inotifyFD_{-1};
    int watchDescriptor_{-1};
    std::thread watcherThread_;
    std::atomic<bool> running_{false};
};

#endif // CORESERVICE_H
