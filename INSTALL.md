# Installation
## Cloning repo

```
git clone https://github.com/Supersonic12/CayPPer
cd CayPPer
```

## Build Instructions

This project uses CMake + Ninja for building from source.
General build steps:

```
cmake -B build/ -G Ninja <backend-option>
cmake --build build/
```

Available backend options:

```
-DHYPRLAND_BACKEND=ON
-DSWAY_BACKEND=ON
-DKDE_BACKEND=ON
-DGNOME_BACKEND=ON
-DXFCE_BACKEND=ON
-DXELEVEN_BACKEND=ON
```

By default they all come as OFF

### Arch Linux based

Overall dependencies in pacman repos:

```
sudo pacman -Syu qt6-base qt6-declarative cmake gcc ninja
```

Environment specific dependencies are below.

#### Hyprland

```
sudo pacman -S hyprpaper
cmake -B build/ -G Ninja -DHYPRLAND_BACKEND=ON && cmake --build build/
```

#### Sway

```
sudo pacman -S nlohmann-json
cmake -B build/ -G Ninja -DSWAY_BACKEND=ON && cmake --build build/
```

#### KDE

> [!CAUTION]
> In KDE opening dialogs use mostly QT's own dialog system. 
> This may cause bugs whenever the dialog state changes (opening, directory navigation, etc.).
> This warnings are harmless, however they may slightly impact user experience. 
> Currently waiting for fix of this problem.

```
sudo pacman -S sdbus-cpp nlohmann-json
cmake -B build/ -G Ninja -DCMAKE_INSTALL_PREFIX=/usr -DKDE_BACKEND=ON && sudo cmake --build build/ --target install
```

#### GNOME

No additional dependencies needed.
```
cmake -B build/ -G Ninja -DGNOME_BACKEND=ON && cmake --build build/
```

#### XFCE

```
sudo pacman -S xfconf
cmake -B build/ -G Ninja -DXFCE_BACKEND=ON && cmake --build build/
```

#### X11 based any wm(without DE)

```
sudo pacman -S xwallpaper
cmake -B build/ -G Ninja -DXELEVEN_BACKEND=ON && cmake --build build/
```

### Debian based

Overall dependencies:
```
sudo apt install build-essential cmake ninja-build \
qt6-base-dev qt6-declarative-dev \
qml6-module-qtquick-controls libqt6quickcontrols2-6 \
qt6-base-dev-tools
```

> [!CAUTION]
> On Debian-based systems built process may trigger bunch of warnings like `someplugin could not be linked.` . These are known upstream bugs. 
> Even though there are warnings, they are harmless and user can safely ignore them. 

#### XFCE

```
sudo apt install xfconf
cmake -B build/ -G Ninja -DXFCE_BACKEND=ON && cmake --build build/
```

#### KDE

> [!CAUTION]
> In KDE opening dialogs use mostly QT's own dialog system. 
> This may cause bugs whenever the dialog state changes (opening, directory navigation, etc.).
> This warnings again is harmless, but they might influence user experience. 
> Currently waiting for fix of this problem.

```
sudo apt install libsdbus-c++-dev nlohmann-json3-dev
cmake -B build/ -G Ninja -DCMAKE_INSTALL_PREFIX=/usr -DKDE_BACKEND=ON && sudo cmake --build build/ --target install
```

#### GNOME

No additional dependencies needed
```
cmake -B build/ -G Ninja -DGNOME_BACKEND=ON && cmake --build build/
```

#### Sway

```
sudo apt install nlohmann-json3-dev
cmake -B build/ -G Ninja -DSWAY_BACKEND=ON && cmake --build build/
```

#### X11 based WMs

```
sudo apt install xwallpaper
cmake -B build/ -G Ninja -DXELEVEN_BACKEND=ON && cmake --build build/
```


