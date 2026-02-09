#ifndef FILLMODECONVERTER_H
#define FILLMODECONVERTER_H
#include <string>
#include <iostream>
#include "fillmode.h"
#include <optional>

inline std::string fromSwayModetoString(FillModeSway mode)
{
    switch(mode){
    case FillModeSway::Center:      return "center";
    case FillModeSway::Fill:        return "fill";
    case FillModeSway::Fit:         return "fit";
    case FillModeSway::Stretch:     return "stretch";
    case FillModeSway::Tile:        return "tile";
    };
    return "unknown";
}
inline std::string fromHyprModetoString(FitModeHyprland mode){
    switch(mode){
    case FitModeHyprland::Fill:     return "fill";
    case FitModeHyprland::Contain:  return "contain";
    case FitModeHyprland::Cover:    return "cover";
    case FitModeHyprland::Tile:     return "tile";
    }
}
inline std::string fromXWallModetoString(FillModeXWall mode){
    switch(mode){
    case FillModeXWall::Center:     return "--center";
    case FillModeXWall::Focus:      return "--focus";
    case FillModeXWall::Maximize:   return "--maximize";
    case FillModeXWall::Stretch:    return "--stretch";
    case FillModeXWall::Tile:       return "--tile";
    case FillModeXWall::Zoom:       return "--zoom";
    }
}

inline std::optional<FillModeXWall> mapToXWall(FillMode mode){
    switch(mode){
    case FillMode::Center:          return FillModeXWall::Center;
    case FillMode::Focus:           return FillModeXWall::Focus;
    case FillMode::Maximize:        return FillModeXWall::Maximize;
    case FillMode::Tile:            return FillModeXWall::Tile;
    case FillMode::Stretch:         return FillModeXWall::Stretch;
    case FillMode::Zoom:            return FillModeXWall::Zoom;
    default:
        std::cerr<<"Unsupported X11 fill mode\n";
        return std::nullopt;
    }
}
inline std::optional<FillModeSway> mapToSway(FillMode mode){
    switch(mode){
    case FillMode::Center: return FillModeSway::Center;
    case FillMode::Tile: return FillModeSway::Tile;
    case FillMode::Stretch: return FillModeSway::Stretch;
    case FillMode::Fill: return FillModeSway::Fill;
    case FillMode::Fit: return FillModeSway::Fit;
    default:
        std::cerr<<"Unsupported Sway fill mode\n";
        return std::nullopt;
    }
}

inline std::optional<FitModeHyprland> mapToHyprland(FillMode mode){
    switch(mode){
    case FillMode::Contain: return FitModeHyprland::Contain;
    case FillMode::Tile: return FitModeHyprland::Tile;
    case FillMode::Cover: return FitModeHyprland::Cover;
    case FillMode::Fill: return FitModeHyprland::Fill;
    default:
        std::cerr<<"Unsupported Hyprland fill mode\n";
        return std::nullopt;
    }
}
inline FillMode toFillMode(std::string mode){
    if(mode=="Fill"){
        return FillMode::Fill;
    }else if(mode=="Fit"){
        return FillMode::Fit;
    }else if(mode=="Stretch"){
        return FillMode::Stretch;
    }else if(mode=="Tile"){
        return FillMode::Tile;
    }else if(mode=="Center"){
        return FillMode::Center;
    }else if(mode=="Contain"){
        return FillMode::Contain;
    }else if(mode=="Cover"){
        return FillMode::Cover;
    }else if(mode=="Focus"){
        return FillMode::Focus;
    }else if(mode=="Maximize"){
        return FillMode::Maximize;
    }else{
        return FillMode::Zoom;
    }
}
#endif // FILLMODECONVERTER_H
