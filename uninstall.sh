#!/bin/bash
# ==============================================================================
# CayPPer Uninstaller
# ==============================================================================
set -euo pipefail

# 1. Define the Manifest
# Paths match CMakeLists install destinations exactly.
# Backends are MODULE libraries (no 'lib' prefix, plain .so).
TARGETS=(
    # Binary
    "/usr/lib/caypper/caypper"
    "/usr/local/lib/caypper/caypper"

    # Core shared library
    "/usr/lib/caypper/libcayppercore.so"
    "/usr/local/lib/caypper/libcayppercore.so"

    # Backend modules (MODULE = no 'lib' prefix)
    "/usr/lib/caypper/gnomechanger.so"
    "/usr/lib/caypper/hyprlandchanger.so"
    "/usr/lib/caypper/plasmachanger.so"
    "/usr/lib/caypper/swaychanger.so"
    "/usr/lib/caypper/xfcechanger.so"
    "/usr/lib/caypper/xelevenchanger.so"
    "/usr/local/lib/caypper/gnomechanger.so"
    "/usr/local/lib/caypper/hyprlandchanger.so"
    "/usr/local/lib/caypper/plasmachanger.so"
    "/usr/local/lib/caypper/swaychanger.so"
    "/usr/local/lib/caypper/xfcechanger.so"
    "/usr/local/lib/caypper/xelevenchanger.so"

    # Assets
    "/usr/share/caypper/script.js"
    "/usr/local/share/caypper/script.js"
    "/usr/share/applications/caypper.desktop"
    "/usr/local/share/applications/caypper.desktop"
    "/usr/share/icons/hicolor/256x256/apps/caypper.png"
    "/usr/local/share/icons/hicolor/256x256/apps/caypper.png"
    "/usr/share/icons/hicolor/scalable/apps/caypper.svg"
    "/usr/local/share/icons/hicolor/scalable/apps/caypper.svg"
)

# 2. Define User Config (handled separately to avoid sudo issues)
REAL_USER_CONFIG="/home/${SUDO_USER:-$USER}/.config/caypper/caypper.ini"

# --- Logic ---
if [[ $EUID -ne 0 ]]; then
    echo "Error: Please run as root (sudo ./uninstall.sh)"
    exit 1
fi

echo "--- Starting CayPPer Removal ---"

# Loop through the system targets
for item in "${TARGETS[@]}"; do
    if [ -e "$item" ] || [ -L "$item" ]; then
        if rm -f "$item"; then
            echo "[REMOVED] $item"
        else
            echo "[ERROR] Could not remove $item"
        fi
    fi
done

# 3. Clean up empty directories
REMAINING_DIRS=(
    "/usr/lib/caypper"
    "/usr/local/lib/caypper"
    "/usr/share/caypper"
    "/usr/local/share/caypper"
)

for dir in "${REMAINING_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        rmdir "$dir" 2>/dev/null && echo "[CLEANED] Directory $dir" || true
    fi
done

# 4. Optional Config Removal
if [ -f "$REAL_USER_CONFIG" ]; then
    echo "------------------------------------------------"
    read -p "Found user config at $REAL_USER_CONFIG. Remove it? [y/N] " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -f "$REAL_USER_CONFIG"
        echo "[REMOVED] Config file."
    fi
fi

echo "--- Uninstallation Complete ---"
