#ifndef GNOME_CHANGER_H
#define GNOME_CHANGER_H
#include "../../IChanger.h"
class GNOMEChanger:public IChanger{
    //In gnome since there is no reliable way of setting wallpaper to specific monitor at a time
    //this setWallpaper will be decoy option until reliable way is added
    void setWallpaper(std::filesystem::path path, std::vector<std::string> selectedMonitors,FillMode fillMode) override;
    void setWallpaperAll(std::filesystem::path path,FillMode fillMode);
    bool isDarkMode();
};

#endif
