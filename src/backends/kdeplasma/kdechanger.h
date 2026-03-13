#ifndef KDECHANGER_H
#define KDECHANGER_H
#include <vector>
#include <string>
#include <filesystem>
#include "../../cayppercore/IChanger.h"
class KDEChanger : public IBackend
{
public:
    //~KDEChanger();
    void setWallpaper(stateOfMons* newState) override;
    void setWallpaperAll(std::filesystem::path& path,FillMode fillMode);
    std::string getJSPATH();
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
};

#endif // KDECHANGER_H
