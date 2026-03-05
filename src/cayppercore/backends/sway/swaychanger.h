#ifndef SWAYCHANGER_H
#define SWAYCHANGER_H
#include "../../IChanger.h"
#include <unordered_map>
class swayChanger : public IChanger,IMonitorDetect{
public:
    struct monitorState{
        pid_t pid;
        std::string currentWall;
        std::string currentFillMode;
    };
    void setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors, FillMode fillMode) override;
    std::vector<FillMode> supportedModes() const override;
    std::vector<std::string> monitors() const override;
    void killCurrentSwaybgs();
private:
    std::unordered_map<std::string,monitorState> monitorsState_;
};


#endif
