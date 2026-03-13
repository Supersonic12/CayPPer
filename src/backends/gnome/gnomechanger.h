#ifndef GNOME_CHANGER_H
#define GNOME_CHANGER_H
#include "../../cayppercore/IChanger.h"
class GNOMEChanger:public IBackend{
public:
    //~GNOMEChanger() override;
    //In gnome since there is no reliable way of setting wallpaper to specific monitor at a time
    //this setWallpaper will be decoy option until reliable way is added
    void setWallpaper(stateOfMons *newState) override;
    void setWallpaperAll(std::filesystem::path& path,FillMode fillMode);
    bool isDarkMode();
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif
