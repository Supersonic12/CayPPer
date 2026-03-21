#ifndef XFCECHANGER_H
#define XFCECHANGER_H

#include "../../cayppercore/IChanger.h"

class XFCEChanger : public IBackend
{
public:
    //~XFCEChanger() override;
    void setWallpaper(stateOfMons* newState) override;
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif // XFCECHANGER_H
