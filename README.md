# A Waypaper Alternative coded with QT,WX and C++ 

## Dependency Installation
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

If x11 based system

```
sudo apt install \
xwallpaper \
x11-xserver-utils
```

if hyprland

`sudo pacman -Sy hyprpaper`


## Build from source
```
git clone https://github.com/Supersonic12/CayPPer
cd CayPPer
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

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

# To Do
- [x] Make Grid delegates clickable, hoverable
- [x] Add Mouse navigation through grid delegates
- [x] Add Keyboard navigation through grid delegates
- [x] Add enter for choosing a setBackground argument from delegates
- [x] Add a class for setting background in different compositors
- [x] Add functional settings, instead of current decoy ones
- [x] Add options for setting wallpapers. e.g. fill, center ,zoom, tile
- [x] Add monitor options e.g. hdmi, edp-1 or all
- [ ] Add recommendation list for TextField and directory listing
- [ ] Save most used directories and use them when recommending directories during search
- [ ] Add a default directory to show when opening program
- [x] in settings add setting default directory, 
- [x] make default directory saved in config directory
- [x] add vim key navigation support
- [x] add enabling vim key navigation in settings

