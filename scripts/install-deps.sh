#!/bin/bash
# Project: NL-NumEngine
# Script: install-deps.sh
#
# Description:
#   Installs project dependencies based on the detected package manager.
#
# Dependencies:
#   One of: apt, yum, pacman, brew

# Detect package manager
if command -v apt >/dev/null 2>&1; then
    PKG_MANAGER="apt"
    INSTALL_CMD="apt install -y"
elif command -v yum >/dev/null 2>&1; then
    PKG_MANAGER="yum"
    INSTALL_CMD="yum install -y"
elif command -v pacman >/dev/null 2>&1; then
    PKG_MANAGER="pacman"
    INSTALL_CMD="pacman -S --noconfirm"
elif command -v brew >/dev/null 2>&1; then
    PKG_MANAGER="brew"
    INSTALL_CMD="brew install"
else
    echo "Error: No supported package manager found"
    exit 1
fi

# Install dependencies
$INSTALL_CMD cmake make g++ \
    clang-format clang-tidy \
    valgrind gdb \
    gcovr doxygen \
    python3-pip

# Install Python dependencies
pip3 install cmake-format 