#ifndef COMPOSFINDER_H
#define COMPOSFINDER_H
class ComposFinder
{
public:
    enum class Compositor {
        Hyprland,
        Sway,
        Unknown
    };

    ComposFinder();
    bool isWayland();
    Compositor getCompositor();
};

#endif // COMPOSFINDER_H
