#ifndef XELEVEN_CHANGER_H
#define XELEVEN_CHANGER_H
#include "../../IChanger.h"
class XChanger : public IChanger,IMonitorDetect{
    void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode) override;
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif
