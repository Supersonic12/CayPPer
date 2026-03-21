# Installation

## Cloning the Repository

```bash
git clone https://github.com/Supersonic12/CayPPer
cd CayPPer
```

---

## Install via install.sh script

```
sudo ./install.sh
```

---
## How It Works

CayPPer uses a **component-based install system**. All backends are always compiled
together in a single build — no CMake flags required. You then install only the
components you need: `base` (required) plus the component for your desktop environment.

| Component | Environment |
| --- | --- |
| `base` | Required by everyone — core app, libraries, desktop entry |
| `gnome` | GNOME |
| `hyprland` | Hyprland |
| `kde` | KDE Plasma |
| `sway` | Sway |
| `xfce` | XFCE |
| `x11` | X11 (any WM without a DE) |

---

## Dependencies

### Common (required for all builds)

These are needed regardless of your desktop environment.

**Arch:**
```bash
sudo pacman -Syu qt6-base qt6-declarative cmake gcc ninja nlohmann-json
```

**Debian / Ubuntu:**
```bash
sudo apt install build-essential cmake ninja-build \
  qt6-base-dev qt6-declarative-dev \
  qml6-module-qtquick-controls libqt6quickcontrols2-6 \
  qt6-base-dev-tools nlohmann-json3-dev
```

### KDE only (additional)

KDE requires `sdbus-c++` in addition to the common dependencies:

**Arch:**
```bash
sudo pacman -S sdbus-cpp
```

**Debian / Ubuntu:**
```bash
sudo apt install libsdbus-c++-dev
```

> All other backends (GNOME, Hyprland, Sway, XFCE, X11) have no extra dependencies
> beyond the common ones above.

---

## Building

Configure and build in one step — no backend flags needed:

```bash
cmake -B build/ -G Ninja -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build/
```

> [!NOTE]
> `-DCMAKE_INSTALL_PREFIX=/usr` is required so that runtime paths (plugin and
> script locations) resolve correctly after installation.

> [!CAUTION]
> On Debian-based systems, the build may produce warnings such as
> `some plugin could not be linked`. These are known upstream issues and are
> harmless — you can safely ignore them.

---

## Installing

Install `base` plus the component matching your desktop environment:

### GNOME

Runtime dependency: `gsettings` (part of `glib2` on Arch, `libglib2.0-bin` on Debian/Ubuntu — usually pre-installed).

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component gnome
```

### Hyprland

Runtime dependency: `hyprpaper`.

**Arch:**
```bash
sudo pacman -S hyprpaper
```
**Debian / Ubuntu:**
```bash
sudo apt install hyprpaper
```

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component hyprland
```

### KDE Plasma

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component kde
```

### Sway

Runtime dependency: `swaybg`.

**Arch:**
```bash
sudo pacman -S swaybg
```
**Debian / Ubuntu:**
```bash
sudo apt install swaybg
```

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component sway
```

### XFCE

Runtime dependency: `xfconf`.

**Arch:**
```bash
sudo pacman -S xfconf
```
**Debian / Ubuntu:**
```bash
sudo apt install xfconf
```

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component xfce
```

### X11 (any WM without a DE)

Runtime dependency: `xwallpaper`.

**Arch:**
```bash
sudo pacman -S xwallpaper
```
**Debian / Ubuntu:**
```bash
sudo apt install xwallpaper
```

```bash
sudo cmake --install build/ --component base
sudo cmake --install build/ --component x11
```

---

## Running CayPPer

After installing, launch the app from your application menu or run:

```bash
/usr/lib/caypper/caypper
```

---

## Uninstalling

An uninstall script is installed alongside the app:

```bash
sudo /usr/share/caypper/uninstall.sh
```
