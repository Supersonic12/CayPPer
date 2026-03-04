#ifndef SWAYCHANGER_H
#define SWAYCHANGER_H
#include "../../IChanger.h"
class swayChanger : public IChanger{
public:
    void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode) override;
    std::vector<FillMode> supportedModes() const override;
};

#endif
