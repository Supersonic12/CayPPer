#ifndef ENVVARDETECTOR_H
#define ENVVARDETECTOR_H
class EnvVarDetector
{
public:
    enum class Compositor {
        Hyprland,
        Sway,
        Unknown
    };

    EnvVarDetector();
    bool isWayland();
    bool isXFCE();
    Compositor getCompositor();
};

#endif // ENVVARDETECTOR_H
