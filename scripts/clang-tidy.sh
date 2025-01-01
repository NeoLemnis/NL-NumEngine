#!/bin/bash

# Exit on any error
set -e

# Script configuration
VERBOSE=0
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "1")
FIX_ERRORS=0
HEADER_FILTER=".*"
BUILD_DIR="."

# Help message
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS]

Clang-tidy options:
    --fix           Apply suggested fixes
    --header-filter Pattern to filter headers (default: ".*")
    --build-dir     Directory containing compile_commands.json (default: ".")
    --verbose       Show detailed output
    --jobs N        Number of parallel jobs (default: $JOBS)
    --help          Show this help message

Example:
    $(basename "$0") --fix --verbose    # Fix issues with detailed output
    $(basename "$0") --jobs 4           # Run checks with 4 parallel jobs
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
        --fix)
            FIX_ERRORS=1
            shift
            ;;
        --header-filter)
            HEADER_FILTER="$2"
            shift 2
            ;;
        --build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        --jobs)
            JOBS="$2"
            shift 2
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

# Check if compile_commands.json exists
COMPILE_COMMANDS="$BUILD_DIR/compile_commands.json"
if [ ! -f "$COMPILE_COMMANDS" ]; then
    echo "Error: compile_commands.json not found in $BUILD_DIR"
    echo "Make sure to:"
    echo "1. Run CMake with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    echo "2. Specify correct build directory with --build-dir option"
    exit 1
fi

# Check if clang-tidy is installed
if ! command -v run-clang-tidy >/dev/null 2>&1; then
    echo "Error: run-clang-tidy not found"
    echo "Please install clang-tidy and ensure it's in your PATH"
    exit 1
fi

# Construct the clang-tidy command
CMD="run-clang-tidy"
CMD="$CMD -p $BUILD_DIR"
CMD="$CMD -header-filter=\"$HEADER_FILTER\""
CMD="$CMD -j $JOBS"
[ $FIX_ERRORS -eq 1 ] && CMD="$CMD -fix"
[ $VERBOSE -eq 0 ] && CMD="$CMD -quiet"

# Run clang-tidy
log "Running: $CMD"
eval "$CMD"

# Check exit status
if [ $? -eq 0 ]; then
    log "Clang-tidy completed successfully"
else
    echo "Error: Clang-tidy failed"
    exit 1
fi
