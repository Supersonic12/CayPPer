#ifndef CHANGER_H
#define CHANGER_H
#include<filesystem>
class Changer
{
public:
    Changer();
    int runXWallpaper(std::filesystem::path chosenPaper);
    int runHyprland(std::filesystem::path chosenPaper);
    int runSway(std::filesystem::path chosenPaper);
};

#endif // CHANGER_H
