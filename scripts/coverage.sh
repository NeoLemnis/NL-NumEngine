#!/bin/bash
# Coverage Report Generator Script
#
# This script generates code coverage reports for the project using gcovr.
# It performs the following steps:
# 1. Cleans previous coverage data
# 2. Rebuilds the project with coverage flags
# 3. Runs tests to generate coverage data
# 4. Generates HTML and XML reports
#
# Dependencies:
# - gcovr
# - cmake
# - g++ with coverage support
#
# Usage: ./scripts/coverage.sh [OPTIONS]
#   --verbose     Show detailed output
#   --help        Show this help message

# Exit on any error
set -e

# Script configuration
VERBOSE=0
BUILD_DIR="build"
COVERAGE_DIR="coverage"

# Help message
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS]

Coverage options:
    --verbose     Show detailed output
    --help        Show this help message

Example:
    $(basename "$0") --verbose    # Generate coverage with detailed output
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
        --verbose)
            VERBOSE=1
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

# Check for gcovr
if ! command -v gcovr &> /dev/null; then
    echo "Error: gcovr not found"
    echo "Please install gcovr: pip install gcovr"
    exit 1
fi

# Create output directory for coverage reports
mkdir -p "$COVERAGE_DIR"

# Clean previous coverage data
log "Cleaning previous coverage data..."
find . -name "*.gcda" -delete
find . -name "*.gcno" -delete

# Clean and rebuild with coverage flags
log "Rebuilding with coverage flags..."
rm -rf "$BUILD_DIR"
cmake -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="--coverage" \
      -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build "$BUILD_DIR"

# Run tests to generate coverage data
log "Running tests to generate coverage data..."
cd "$BUILD_DIR" && ./runTests
cd ..

# Run gcovr to generate HTML coverage report
log "Generating coverage reports..."
gcovr --html-details "$COVERAGE_DIR/coverage.html" \
      --xml "$COVERAGE_DIR/coverage.xml" \
      --exclude-unreachable-branches \
      --exclude-throw-branches \
      -r . \
      --filter="src/.*" \
      --filter="include/.*" \
      --filter="main\.cpp" \
      --exclude=".*_test\.cpp" \
      --exclude="tests/.*" \
      --exclude="build/.*" \
      --exclude="/usr/.*" \
      --exclude=".*/googletest/.*" \
      --print-summary

# Check if reports were generated successfully
if [ $? -eq 0 ]; then
    echo "Coverage reports generated successfully:"
    echo "- HTML report: $COVERAGE_DIR/coverage.html"
    echo "- XML report:  $COVERAGE_DIR/coverage.xml"
    
    # Print coverage summary from HTML report
    echo -e "\nCoverage Summary:"
    grep -A 4 "Lines:" "$COVERAGE_DIR/coverage.html" | sed 's/<[^>]*>//g' | grep -v "^--"
else
    echo "Error generating coverage reports"
    exit 1
fi
