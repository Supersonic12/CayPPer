#ifndef KDECHANGER_H
#define KDECHANGER_H
#include <vector>
#include <string>
#include <filesystem>
#include "../../IChanger.h"
class KDEChanger : public IChanger
{
public:
    void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors,FillMode fillMode) override;
    void setWallpaperAll(std::filesystem::path path,FillMode fillMode);
};

#endif // KDECHANGER_H
