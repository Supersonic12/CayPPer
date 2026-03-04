#ifndef ICHANGER_H
#define ICHANGER_H
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include "domainExpansion/fillmode.h"
#include "domainExpansion/fillmodeconverter.h"
class IChanger {
public:
    virtual ~IChanger()=default;
    virtual void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors,FillMode fillMode)=0;
    virtual std::vector<FillMode> supportedModes() const=0;

};

#endif // ICHANGER_H
