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
    // fallback
    std::cerr << "Invalid Hyprland fill mode\n";
    return "fill";  // safe default
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
    // fallback
    std::cerr << "Invalid XWallpaper fill mode\n";
    return "--focus";  // safe default
}
inline std::string fromXFCEModetoString(FillModeXFCE mode){
    switch(mode){
    case FillModeXFCE::Center:              return "1";
    case FillModeXFCE::Zoom:                return "2";
    case FillModeXFCE::Scaled:              return "4";
    case FillModeXFCE::Stretch:             return "3";
    case FillModeXFCE::Spanning_Screens:    return "5";
    }
    // fallback
    std::cerr << "Invalid XFCE fill mode\n";
    return "4";  // safe default
}
inline int fromKDEModetoInt(FillModeKDE mode){
    switch(mode){

    case FillModeKDE::Scaled:                return 0;
    case FillModeKDE::ScaledKeepAspect:      return 1;
    case FillModeKDE::ScaledCropped:         return 2;
    case FillModeKDE::Tile:                  return 3;
    case FillModeKDE::TileVertically:        return 4;
    case FillModeKDE::TileHorizontally:      return 5;
    case FillModeKDE::Center:                return 6;

    default:
        return 4;
    }
}
inline std::string fromGNOMEModetoString(FillModeGNOME mode){
    switch(mode){
    case FillModeGNOME::Center: return "centered";
    case FillModeGNOME::Scaled: return "scaled";
    case FillModeGNOME::Tile:   return "wallpaper";
    case FillModeGNOME::Stretch:return "stretched";
    case FillModeGNOME::Spanning_Screens:return "spanned";
    default:    return "zoom";
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
    case FillMode::Center:  return FillModeSway::Center;
    case FillMode::Tile:    return FillModeSway::Tile;
    case FillMode::Stretch: return FillModeSway::Stretch;
    case FillMode::Fill:    return FillModeSway::Fill;
    case FillMode::Fit:     return FillModeSway::Fit;
    default:
        std::cerr<<"Unsupported Sway fill mode\n";
        return std::nullopt;
    }
}

inline std::optional<FitModeHyprland> mapToHyprland(FillMode mode){
    switch(mode){
    case FillMode::Contain: return FitModeHyprland::Contain;
    case FillMode::Tile:    return FitModeHyprland::Tile;
    case FillMode::Cover:   return FitModeHyprland::Cover;
    case FillMode::Fill:    return FitModeHyprland::Fill;
    default:
        std::cerr<<"Unsupported Hyprland fill mode\n";
        return std::nullopt;
    }
}
inline std::optional<FillModeXFCE>  mapToXFCE(FillMode mode){
    switch(mode){
    case FillMode::Center:              return FillModeXFCE::Center;
    case FillMode::Spanning_Screens:    return FillModeXFCE::Spanning_Screens;
    case FillMode::Stretch:             return FillModeXFCE::Stretch;
    case FillMode::Zoom:                return FillModeXFCE::Zoom;
    case FillMode::Scaled:              return FillModeXFCE::Scaled;
    default:
        std::cerr<<"Unsupported XFCE fill mode\n";
        return std::nullopt;
    }
}
inline std::optional<FillModeKDE>   mapToKDE(FillMode mode){
    switch(mode){
    case FillMode::Center:          return FillModeKDE::Center;
    case FillMode::Scaled:          return FillModeKDE::Scaled;
    case FillMode::ScaledKeepAspect:return FillModeKDE::ScaledKeepAspect;
    case FillMode::Tile:            return FillModeKDE::Tile;
    case FillMode::ScaledCropped:   return FillModeKDE::ScaledCropped;
    case FillMode::TileVertically:  return FillModeKDE::TileVertically;
    case FillMode::TileHorizontally:return FillModeKDE::TileHorizontally;
    default:
        std::cerr<<"Unsupported KDE fill mode\n";
        return std::nullopt;
    }
}
inline std::optional<FillModeGNOME> mapToGNOME(FillMode mode){
    switch(mode){
    case FillMode::Center:      return FillModeGNOME::Center;
    case FillMode::Scaled:      return FillModeGNOME::Scaled;
    case FillMode::Spanning_Screens: return FillModeGNOME::Spanning_Screens;
    case FillMode::Tile:        return FillModeGNOME::Tile;
    case FillMode::Stretch:     return FillModeGNOME::Stretch;
    case FillMode::Zoom:        return FillModeGNOME::Zoom;
    default:
        std::cerr<<"Unsupported GNOME fill mode\n";
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
    }else if(mode=="Scaled"){
        return FillMode::Scaled;
    }else if(mode=="SpanningScreens"){
        return FillMode::Spanning_Screens;
    }else if(mode=="Tile(Horizontally)"){
        return FillMode::TileHorizontally;
    }else if(mode=="Tile(Vertically)"){
        return FillMode::TileVertically;
    }else if(mode=="Scaled(Keep Ratio)"){
        return FillMode::ScaledKeepAspect;
    }else if(mode=="Scaled(Crop)"){
        return FillMode::ScaledCropped;
    }
    else{
        return FillMode::Zoom;
    }
}
#endif // FILLMODECONVERTER_H
