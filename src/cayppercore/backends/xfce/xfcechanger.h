#ifndef XFCECHANGER_H
#define XFCECHANGER_H

#include "../../IChanger.h"

class XFCEChanger : public IChanger
{
public:
    void setWallpaper(std::filesystem::path path,std::vector<std::string> monitors,FillMode fillMode) override;
    std::vector<FillMode> supportedModes() const override;
};

#endif // XFCECHANGER_H
