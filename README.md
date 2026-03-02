# A Waypaper Alternative coded with QT,WX and C++ 

## Dependency Installation
**ON (K)Ubuntu**

```
sudo apt install \
    build-essential \
    cmake \
    ninja-build \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-base-dev-tools \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qt6-wayland 
```

If X11 based WM(Tested on DWM)

```
sudo apt install \
xwallpaper \
x11-xserver-utils
```

if Hyprland

`sudo pacman -Sy hyprpaper`

if KDE

```
sudo apt install libsdbus-c++-dev nlohman-json3-dev
```

## Build from source

```
git clone https://github.com/Supersonic12/CayPPer
cd CayPPer
mkdir build
cd build
cmake -G Ninja -DCMAKE_INSTALL_PREFIX=/usr ..
ninja build install
```

### Options for different environments

```-DKDE_BACKEND ON/OFF
-DGNOME_BACKEND ON/OFF
-DHYPRLAND_BACKEND ON/OFF
-DXELEVEN_BACKEND ON/OFF
-DXFCE_BACKEND ON/OFF
```
By default all of them comes OFF

## Vim Keys

### In Main Menu

> gs - go Settings menu \
> gw - open folder dialog \
> gr - focus to wallpaper grid \
> gm - focus to fill mode changer 

### in Grid

> h - go left \
> l - go right \
> j - go down \
> k - go up \
> gg - go uppermost wallpaper \
> G - go bottommost wallpaper \
> f - set highlighted wallpaper 

### in Settings

> gn - go to Main menu \
> gW - open default folder choice dialog \
> gc - open config folder choice dialog \
> gv - turn of vim keys
