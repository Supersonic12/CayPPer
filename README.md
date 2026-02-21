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
    qt6-wayland \
    x11-xserver-utils \
    xwallpaper
```
## Build from source
```
git clone https://github.com/Supersonic12/CayPPer
cd CayPPer
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja

```
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

