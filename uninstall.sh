#!/bin/bash
MANIFEST="/usr/share/caypper/install_manifest.txt"
MANIFEST2="/usr/local/share/caypper/install_manifest.txt"
if [ ! -f "$MANIFEST" ] && [ ! -f "$MANIFEST2" ]; then
    echo "CayPPer does not appear to be installed"
    exit 1
fi

[ -f "$MANIFEST" ] && sudo xargs rm -f < "$MANIFEST" && sudo rm -f "$MANIFEST"
[ -f "$MANIFEST2" ] && sudo xargs rm -f < "$MANIFEST2" && sudo rm -f "$MANIFEST2"
echo "CayPPer uninstalled successfully"
