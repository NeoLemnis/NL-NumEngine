#!/bin/bash
# Project: NL-NumEngine
# Script: run.sh
#
# Description:
#   Runs the project executable with various debugging and profiling options.
#
# Dependencies:
#   - valgrind (optional)
#   - gdb (optional)
#   - perf (optional)
#   - heaptrack (optional)
#
# Usage: ./scripts/run.sh [OPTIONS] [-- PROGRAM_ARGS]

# Exit on any error
set -e

# Script configuration
VERBOSE=0
BUILD_DIR="build"
EXECUTABLE="NL-NumEngine"
VALGRIND=0
GDB=0
ARGS=""

# Add new configuration options
PERF=0
HEAPTRACK=0
CALLGRIND=0
MASSIF=0

# Help message
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS] [-- PROGRAM_ARGS]

Run options:
    --valgrind     Run with valgrind for memory checking
    --gdb          Run with gdb for debugging
    --build-dir    Specify build directory (default: "build")
    --verbose      Show detailed output
    --help         Show this help message

Additional arguments can be passed to the program after --

Examples:
    $(basename "$0")                     # Normal run
    $(basename "$0") --valgrind          # Run with valgrind
    $(basename "$0") --gdb               # Run with gdb
    $(basename "$0") -- --input file.txt # Pass arguments to program
EOF
    exit 0
}

# Function to echo only if verbose is enabled
log() {
    if [ $VERBOSE -eq 1 ]; then
        echo "$1"
    fi
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --valgrind)
            VALGRIND=1
            shift
            ;;
        --gdb)
            GDB=1
            shift
            ;;
        --build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        --help)
            show_help
            ;;
        --)
            shift
            ARGS="$*"
            break
            ;;
        *)
            echo "Error: Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory '$BUILD_DIR' not found"
    echo "Please run the build script first"
    exit 1
fi

# Change to the build directory
echo "Changing to build directory ..."
cd build
echo -e "Changed to build directory\n"

# If the executable does not exist, exit the script
echo "Checking for executable ..."
if [ ! -f "NL-NumEngine" ]; then
    echo "Error: Executable not found."
  exit 1
fi
echo -e "Executable found\n"

echo -e "Running the executable ...\n"

# Add performance profiling
if [ $PERF -eq 1 ]; then
    if ! command -v perf >/dev/null 2>&1; then
        echo "Error: perf not found"
        exit 1
    fi
    CMD="perf record -g ./$EXECUTABLE $ARGS"
fi

# Add heap profiling
if [ $HEAPTRACK -eq 1 ]; then
    if ! command -v heaptrack >/dev/null 2>&1; then
        echo "Error: heaptrack not found"
        exit 1
    fi
    CMD="heaptrack ./$EXECUTABLE $ARGS"
fi

# Add callgrind profiling
if [ $CALLGRIND -eq 1 ]; then
    CMD="valgrind --tool=callgrind ./$EXECUTABLE $ARGS"
fi

# Add memory profiling
if [ $MASSIF -eq 1 ]; then
    CMD="valgrind --tool=massif ./$EXECUTABLE $ARGS"
fi

# Run the executable
$CMD