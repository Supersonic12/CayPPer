#ifndef HYPRCHANGER_H
#define HYPRCHANGER_H
#include "../../cayppercore/IChanger.h"
class HyprChanger: public IBackend
{
public:
    //~HyprChanger();
    void setWallpaper(stateOfMons* newState) override;
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif
