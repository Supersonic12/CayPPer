#ifndef CHANGER_H
#define CHANGER_H
#include<filesystem>
#include<vector>
#include<string>
#include "domainExpansion/fillmode.h"

class Changer
{
public:
    Changer();
    int runXWallpaper(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeXWall fillmode);
    int runHyprland(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FitModeHyprland fillmode);
    int runSway(std::filesystem::path chosenPaper,std::vector<std::string> selectedMonitors, FillModeSway fillmode);
};

#endif // CHANGER_H
