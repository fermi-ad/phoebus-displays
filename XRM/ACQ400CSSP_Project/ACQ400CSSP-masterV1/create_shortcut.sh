#!/usr/bin/env bash

ROOT_DIR=$(dirname "$(realpath "$0")")

cat << EOF > ~/Desktop/ACQ400CSSP.desktop
[Desktop Entry]
Type=Application
Name=ACQ400CSSP
Exec=$ROOT_DIR/ACQ400CSSP_launcher.py
Icon=$ROOT_DIR/ACQ400CSSP_icon.png
Terminal=true
EOF