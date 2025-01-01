#!/bin/bash
# Project: NL-NumEngine
# Script: build.sh
#
# Description:
#   Builds the project using CMake and Make with configurable options.
#   Supports debug and release builds, parallel compilation, and 
#   generates compilation database for tools.
#
# Dependencies:
#   - cmake (3.10 or higher)
#   - make/ninja
#   - C++ compiler (g++/clang++)
#
# Usage: ./scripts/build.sh [OPTIONS]
#   --debug         Build with debug symbols and no optimization
#   --production    Build with optimizations (default)
#   --verbose       Show detailed build output
#   --jobs N        Number of parallel jobs (default: auto)
#   --clean        Clean build directory before building
#   --ninja        Use Ninja build system instead of Make
#   --help         Show this help message

# Exit on any error
set -e

# Script configuration
BUILD_TYPE="Release"
VERBOSE=0
CLEAN=0
USE_NINJA=0
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "1")

# Help message
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS]

Build options:
    --debug         Build with debug symbols and no optimization
    --production    Build with optimizations (default)
    --verbose       Show detailed build output
    --jobs N       Number of parallel jobs (default: $JOBS)
    --clean        Clean build directory before building
    --ninja        Use Ninja build system instead of Make
    --help         Show this help message

Example:
    $(basename "$0") --debug --verbose    # Debug build with verbose output
    $(basename "$0") --jobs 4             # Production build with 4 parallel jobs
EOF
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --production)
            BUILD_TYPE="Release"
            shift
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        --jobs)
            JOBS="$2"
            shift 2
            ;;
        --clean)
            CLEAN=1
            shift
            ;;
        --ninja)
            USE_NINJA=1
            shift
            ;;
        --help)
            show_help
            ;;
        *)
            echo "Error: Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Function to echo only if verbose is enabled
log() {
    if [ $VERBOSE -eq 1 ]; then
        echo "$1"
    fi
}

# Check if the build directory exists, if not create it
if [ ! -d "build" ]; then
    log "Creating build directory ..."
    mkdir build
    log "Build directory created"
fi

# Change to the build directory
log "Changing to build directory ..."
cd build
log "Changed to build directory"

# Add clean build option
if [ $CLEAN -eq 1 ]; then
    log "Cleaning build directory..."
    rm -rf build
    mkdir build
fi

# Add Ninja support
if [ $USE_NINJA -eq 1 ]; then
    CMAKE_GENERATOR="-GNinja"
    BUILD_COMMAND="ninja"
else
    CMAKE_GENERATOR=""
    BUILD_COMMAND="make"
fi

# Run cmake with specified build type and compile commands export
log "Running cmake with BUILD_TYPE=$BUILD_TYPE ..."
if ! cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $CMAKE_GENERATOR ..; then
    echo "CMAKE failed!"
    exit 1
fi
log "CMake ran successfully"

# Handle compile_commands.json
log "Moving compile_commands.json to root directory ..."
if [ -f "../compile_commands.json" ]; then
    rm ../compile_commands.json
fi
mv compile_commands.json ..
log "Moved compile_commands.json to root directory"

# Run make with optional verbose output
log "Running $BUILD_COMMAND with $JOBS jobs..."
if [ $VERBOSE -eq 1 ]; then
    if ! $BUILD_COMMAND VERBOSE=1 -j"$JOBS"; then
        echo "$BUILD_COMMAND failed!"
        exit 1
    fi
else
    if ! $BUILD_COMMAND -j"$JOBS"; then
        echo "$BUILD_COMMAND failed!"
        exit 1
    fi
fi
log "$BUILD_COMMAND ran successfully"

echo "Build completed successfully in $BUILD_TYPE mode"
