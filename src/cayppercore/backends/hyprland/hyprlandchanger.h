#ifndef HYPRCHANGER_H
#define HYPRCHANGER_H
#include "../../IChanger.h"
class HyprChanger: public IChanger
{
    void setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors, FillMode fillMode) override;
    std::vector<FillMode> supportedModes() const override;
};

#endif
