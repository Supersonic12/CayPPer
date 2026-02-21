#ifndef FILLMODE_H
#define FILLMODE_H

enum class FillMode{
    Fill,
    Fit,
    Stretch,
    Tile,
    Center,
    Contain,
    Cover,
    Focus,
    Maximize,
    Scaled,
    Spanning_Screens,
    Zoom
};
enum class FillModeSway{
    Fill,
    Fit,
    Stretch,
    Tile,
    Center
};
enum class FitModeHyprland{
    Contain,
    Cover,
    Tile,
    Fill
};
enum class FillModeXWall{
    Center,
    Focus,
    Maximize,
    Stretch,
    Tile,
    Zoom
};
enum class FillModeXFCE{
    Center,
    Stretch,
    Scaled,
    Zoom,
    Spanning_Screens,
};

#endif // FILLMODE_H
