#include "changerFactory.h"
#include "x11/changerX.h"
#include "kdeplasma/kdechanger.h"
#include "gnome/gnomechanger.h"
#include "xfce/xfcechanger.h"
#include "hyprland/hyprlandchanger.h"
#include "sway/swaychanger.h"

std::unique_ptr<IChanger>
ChangerFactory::create(EnvVarDetector::Compositor compositor, bool isWayland){
    if(!isWayland){
        if(compositor==EnvVarDetector::Compositor::XFCE){
            return std::make_unique<XFCEChanger>();
        }
        return std::make_unique<XChanger>();
    }
    else{
        switch(compositor){
        case EnvVarDetector::Compositor::Hyprland:  return std::make_unique<HyprChanger>();
        case EnvVarDetector::Compositor::KDE:       return std::make_unique<KDEChanger>();
        case EnvVarDetector::Compositor::Sway:      return std::make_unique<swayChanger>();
        case EnvVarDetector::Compositor::GNOME:     return std::make_unique<GNOMEChanger>();
        default:
            return nullptr;
        }
    }
}
