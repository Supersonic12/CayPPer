#ifndef ENVVARDETECTOR_H
#define ENVVARDETECTOR_H
#include <string>
class EnvVarDetector
{
public:
    enum class Compositor {
        Hyprland,
        Sway,
        KDE,
        GNOME,
        XFCE,
        X11,
        Unknown
    };

    EnvVarDetector();
    bool isWayland();
    bool isX11();
    bool xdgContains(const std::string &token);
    Compositor getCompositor();
};

#endif // ENVVARDETECTOR_H
