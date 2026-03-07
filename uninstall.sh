#!/bin/bash

# ==============================================================================
# CayPPer Uninstaller
# ==============================================================================
set -euo pipefail

# 1. Define the Manifest
# We include everything: binaries, core libs, all possible backends, and assets.
TARGETS=(
    "/usr/local/bin/caypper"
    "/usr/local/lib/libcayppercore.so"
    "/usr/local/share/caypper/script.js"
    "/usr/local/lib/libxelevenchanger.so"
    "/usr/local/lib/libswaychanger.so"
    "/usr/local/lib/libplasmawallchanger.so"
    "/usr/local/lib/libxfcewallchanger.so"
    "/usr/local/lib/libgnomewallchanger.so"
    "/usr/local/lib/libhyprlandchanger.so"

    "/usr/bin/caypper"
    "/usr/lib/libcayppercore.so"
    "/usr/share/caypper/script.js"
    "/usr/lib/libxelevenchanger.so"
    "/usr/lib/libswaychanger.so"
    "/usr/lib/libhyprlandchanger.so"
    "/usr/lib/libgnomewallchanger.so"
    "/usr/lib/libplasmawallchanger.so"
    "/usr/lib/libxfcewallchanger.so"

# Debian/Ubuntu/Kali multiarch paths
    "/usr/lib/x86_64-linux-gnu/libcayppercore.so"
    "/usr/lib/x86_64-linux-gnu/libxelevenchanger.so"
    "/usr/lib/x86_64-linux-gnu/libswaychanger.so"
    "/usr/lib/x86_64-linux-gnu/libplasmawallchanger.so"
    "/usr/lib/x86_64-linux-gnu/libxfcewallchanger.so"
    "/usr/lib/x86_64-linux-gnu/libgnomewallchanger.so"
    "/usr/lib/x86_64-linux-gnu/libhyprlandchanger.so"
)

# 2. Define User Config (handled separately to avoid sudo issues)
# We use the real user's home even if run with sudo
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
# Best practice: Don't leave empty 'caypper' folders in /share/
REMAINING_DIRS=(
    "/usr/local/share/caypper"
    "/usr/share/caypper"
)

for dir in "${REMAINING_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        rmdir "$dir" 2>/dev/null && echo "[CLEANED] Directory $dir"
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
