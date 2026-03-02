#include "changerFactory.h"
#ifdef XELEVEN_BACKEND
#include "x11/changerX.h"
#endif
#ifdef KDE_BACKEND
#include "kdeplasma/kdechanger.h"
#endif
#ifdef GNOME_BACKEND
#include "gnome/gnomechanger.h"
#endif
#ifdef XFCE_BACKEND
#include "xfce/xfcechanger.h"
#endif
#ifdef HYPRLAND_BACKEND
#include "hyprland/hyprlandchanger.h"
#endif
#ifdef SWAY_BACKEND
#include "sway/swaychanger.h"
#endif
std::unique_ptr<IChanger>
ChangerFactory::create(EnvVarDetector::Compositor compositor, bool isWayland){
    if(!isWayland){
        #ifdef XFCE_BACKEND
        if(compositor==EnvVarDetector::Compositor::XFCE){
            return std::make_unique<XFCEChanger>();
        }
        #endif
        #ifdef XELEVEN_BACKEND
        return std::make_unique<XChanger>();
        #endif
    }
    else{
        switch(compositor){

#ifdef HYPRLAND_BACKEND

        case EnvVarDetector::Compositor::Hyprland:  return std::make_unique<HyprChanger>();
#endif
#ifdef KDE_BACKEND
        case EnvVarDetector::Compositor::KDE:       return std::make_unique<KDEChanger>();
#endif
#ifdef SWAY_BACKEND
        case EnvVarDetector::Compositor::Sway:      return std::make_unique<swayChanger>();
#endif
#ifdef GNOME_BACKEND
        case EnvVarDetector::Compositor::GNOME:     return std::make_unique<GNOMEChanger>();
#endif
        default:
            return nullptr;
        }
    }
}
