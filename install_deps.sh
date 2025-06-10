#!/bin/bash

echo "Installing SDL2 development dependencies..."
echo "Please run this script with sudo privileges:"
echo "sudo ./install_deps.sh"
echo ""

if [ "$EUID" -ne 0 ]; then
    echo "Please run as root (use sudo)"
    exit 1
fi

apt update
apt install -y build-essential cmake libsdl2-dev libsdl2-ttf-dev pkg-config

echo "Dependencies installed successfully!"
echo "You can now build the project with:"
echo "mkdir -p build && cd build && cmake .. && make"