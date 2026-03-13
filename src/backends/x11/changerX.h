#ifndef XELEVEN_CHANGER_H
#define XELEVEN_CHANGER_H
#include "../../cayppercore/IChanger.h"
class XChanger : public IBackend{
public:  
   // ~XChanger();
    void setWallpaper(stateOfMons* newState) override;
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif
