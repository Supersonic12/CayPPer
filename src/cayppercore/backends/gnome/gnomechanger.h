#ifndef GNOME_CHANGER_H
#define GNOME_CHANGER_H
#include "../../IChanger.h"
class GNOMEChanger:public IChanger{
    void setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors, FillMode fillMode) override;
};

#endif
