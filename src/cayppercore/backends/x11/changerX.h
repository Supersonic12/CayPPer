#ifndef XELEVEN_CHANGER_H
#define XELEVEN_CHANGER_H
#include "../../IChanger.h"
class XChanger : public IChanger{
    void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode) override;
};

#endif
