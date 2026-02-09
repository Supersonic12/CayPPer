#include "coreservice.h"
#include "domainExpansion/fillmodeconverter.h"
coreService::coreService() : isWayland_(composFinder_.isWayland()),
    compositor_(composFinder_.getCompositor())
{
}

bool coreService::isWayland() const{
    return isWayland_;
}
ComposFinder::Compositor coreService::compositor() const{
    return compositor_;
}

std::vector<std::string> coreService::monitors() const{
    if(!isWayland_){
        return {};
    }
    return monitorhandler_.getMonitor(compositor_);
}

void coreService::setWallpaper(const std::filesystem::path& wallPath,std::vector<std::string>& selectedMonitors,FillMode fillMode){
    if(!isWayland_){
        auto mode=mapToXWall(fillMode);
        if(!mode){
            return;
        }
        changer_.runXWallpaper(wallPath,selectedMonitors,*mode);
    }else{
        if(compositor_==ComposFinder::Compositor::Hyprland){
            auto mode=mapToHyprland(fillMode);
            if(!mode){
                return;
            }
            changer_.runHyprland(wallPath,selectedMonitors,*mode);
        }else if(compositor_==ComposFinder::Compositor::Sway){
            auto mode=mapToSway(fillMode);
            if(!mode){
                return;
            }
            changer_.runSway(wallPath,selectedMonitors,*mode);
        }
    }
}

std::vector<FillMode> coreService::supportedModes() const{
    if(!isWayland_){
        return{
            FillMode::Center,
            FillMode::Focus,
            FillMode::Maximize,
            FillMode::Stretch,
            FillMode::Tile,
            FillMode::Zoom
        };
    }else if(compositor_==ComposFinder::Compositor::Hyprland) {
        return{
            FillMode::Contain,
            FillMode::Cover,
            FillMode::Fill,
            FillMode::Tile
        };
    }else if(compositor_==ComposFinder::Compositor::Sway){
        return{
            FillMode::Center,
            FillMode::Fill,
            FillMode::Fit,
            FillMode::Stretch,
            FillMode::Tile
        };
    }else{
        return{};
    }
}
