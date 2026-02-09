#ifndef CORESERVICE_H
#define CORESERVICE_H

#include<string>
#include<filesystem>
#include<vector>

#include "composfinder.h"
#include "monitorhandler.h"
#include "changer.h"
#include "domainExpansion/fillmode.h"

class coreService
{
public:
    coreService();
    bool isWayland() const;
    ComposFinder::Compositor compositor() const;
    std::vector<std::string> monitors() const;
    void setWallpaper(const std::filesystem::path& wallPath,std::vector<std::string>& selectedMonitors,FillMode fillmode);
    std::vector<FillMode> supportedModes() const;

private:
    ComposFinder composFinder_;
    MonitorHandler monitorhandler_;
    Changer changer_;
    bool isWayland_;
    ComposFinder::Compositor compositor_;
};

#endif // CORESERVICE_H
