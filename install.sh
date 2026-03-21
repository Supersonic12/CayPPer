#!/bin/bash
# ==============================================================================
# CayPPer Installer
# ==============================================================================
set -euo pipefail

# --- Colors ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

# --- Helpers ---
info()    { echo -e "${CYAN}[INFO]${RESET} $*"; }
success() { echo -e "${GREEN}[OK]${RESET}   $*"; }
warn()    { echo -e "${YELLOW}[WARN]${RESET} $*"; }
error()   { echo -e "${RED}[ERROR]${RESET} $*"; exit 1; }

# --- Root check ---
if [[ $EUID -ne 0 ]]; then
    error "Please run as root (sudo ./install.sh)"
fi

# --- Must be run from repo root ---
if [[ ! -f "CMakeLists.txt" ]] || ! grep -q "CayPPer" "CMakeLists.txt"; then
    error "Please run this script from inside the cloned CayPPer repository."
fi

echo ""
echo -e "${BOLD}================================================${RESET}"
echo -e "${BOLD}           CayPPer Installer                    ${RESET}"
echo -e "${BOLD}================================================${RESET}"
echo ""

# ==============================================================================
# 1. Detect distro
# ==============================================================================
detect_distro() {
    if [[ -f /etc/os-release ]]; then
        . /etc/os-release
        case "$ID" in
            arch|manjaro|endeavouros|cachyos|garuda) echo "arch" ;;
            debian|ubuntu|linuxmint|pop|kali)        echo "debian" ;;
            *)
                case "$ID_LIKE" in
                    *arch*)   echo "arch" ;;
                    *debian*|*ubuntu*) echo "debian" ;;
                    *) echo "unknown" ;;
                esac
                ;;
        esac
    else
        echo "unknown"
    fi
}

DISTRO=$(detect_distro)

if [[ "$DISTRO" == "unknown" ]]; then
    warn "Could not detect a supported distro (Arch or Debian/Ubuntu)."
    warn "Dependency installation will be skipped — make sure you have:"
    warn "  qt6-base, qt6-declarative, cmake, ninja, nlohmann-json"
fi

info "Detected distro family: ${BOLD}${DISTRO}${RESET}"
echo ""

# ==============================================================================
# 2. Ask the user to choose their desktop environment
# ==============================================================================
echo -e "${BOLD}Select your desktop environment / compositor:${RESET}"
echo ""
echo "  1) GNOME"
echo "  2) Hyprland"
echo "  3) KDE Plasma"
echo "  4) Sway"
echo "  5) XFCE"
echo "  6) X11 (standalone WM, no DE)"
echo ""

while true; do
    read -p "Enter number [1-6]: " DE_CHOICE
    case "$DE_CHOICE" in
        1) DE="gnome";     COMPONENT="gnome";    break ;;
        2) DE="Hyprland";  COMPONENT="hyprland"; break ;;
        3) DE="KDE Plasma" COMPONENT="kde";      break ;;
        4) DE="Sway";      COMPONENT="sway";     break ;;
        5) DE="XFCE";      COMPONENT="xfce";     break ;;
        6) DE="X11";       COMPONENT="x11";      break ;;
        *) warn "Invalid choice. Please enter a number between 1 and 6." ;;
    esac
done

info "Selected: ${BOLD}${DE}${RESET} (component: ${COMPONENT})"
echo ""

# ==============================================================================
# 3. Install dependencies
# ==============================================================================
install_deps_arch() {
    info "Installing common build dependencies (pacman)..."
    pacman -Syu --noconfirm --needed \
        qt6-base qt6-declarative cmake gcc ninja nlohmann-json

    case "$COMPONENT" in
        kde)
            info "Installing KDE-specific dependencies..."
            pacman -S --noconfirm --needed sdbus-cpp
            ;;
        hyprland)
            info "Installing Hyprland runtime dependency (hyprpaper)..."
            pacman -S --noconfirm --needed hyprpaper
            ;;
        sway)
            info "Installing Sway runtime dependency (swaybg)..."
            pacman -S --noconfirm --needed swaybg
            ;;
        xfce)
            info "Installing XFCE runtime dependency (xfconf)..."
            pacman -S --noconfirm --needed xfconf
            ;;
        x11)
            info "Installing X11 runtime dependency (xwallpaper)..."
            pacman -S --noconfirm --needed xwallpaper
            ;;
    esac
}

install_deps_debian() {
    info "Installing common build dependencies (apt)..."
    apt-get update -q
    apt-get install -y --no-install-recommends \
        build-essential cmake ninja-build \
        qt6-base-dev qt6-declarative-dev \
        qml6-module-qtquick-controls libqt6quickcontrols2-6 \
        qt6-base-dev-tools nlohmann-json3-dev

    case "$COMPONENT" in
        kde)
            info "Installing KDE-specific dependencies..."
            apt-get install -y --no-install-recommends libsdbus-c++-dev
            ;;
        hyprland)
            info "Installing Hyprland runtime dependency (hyprpaper)..."
            apt-get install -y --no-install-recommends hyprpaper
            ;;
        sway)
            info "Installing Sway runtime dependency (swaybg)..."
            apt-get install -y --no-install-recommends swaybg
            ;;
        xfce)
            info "Installing XFCE runtime dependency (xfconf)..."
            apt-get install -y --no-install-recommends xfconf
            ;;
        x11)
            info "Installing X11 runtime dependency (xwallpaper)..."
            apt-get install -y --no-install-recommends xwallpaper
            ;;
    esac
}

case "$DISTRO" in
    arch)   install_deps_arch ;;
    debian) install_deps_debian ;;
    *)      warn "Skipping dependency installation on unknown distro." ;;
esac

success "Dependencies ready."
echo ""

# ==============================================================================
# 4. Build
# ==============================================================================
info "Configuring build..."
cmake -B build/ -G Ninja -DCMAKE_INSTALL_PREFIX=/usr

info "Building CayPPer..."
cmake --build build/

success "Build complete."
echo ""

# ==============================================================================
# 5. Install
# ==============================================================================
info "Installing base component..."
cmake --install build/ --component base

info "Installing ${DE} backend component..."
cmake --install build/ --component "$COMPONENT"

success "CayPPer installed successfully."
echo ""
echo -e "${BOLD}You can now launch CayPPer from your application menu"
echo -e "or by running: /usr/bin/caypper/caypper${RESET}"
echo ""
