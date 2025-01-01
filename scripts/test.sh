#!/bin/bash
# Project: NL-NumEngine
# Script: test.sh
#
# Description:
#   Runs project tests with various options including filtering, repetition,
#   memory checking, and test organization features.
#
# Dependencies:
#   - Google Test
#   - valgrind (optional)
#   - lcov (optional for coverage)
#
# Usage: ./scripts/test.sh [OPTIONS]
#   [existing options...]
#   --timeout N     Set test timeout in seconds
#   --xml-output   Generate XML test results
#   --parallel N   Run N tests in parallel
#   --color        Force colored output
#   --no-color     Disable colored output

# Exit on any error
set -e

# Script configuration
VERBOSE=0
BUILD_DIR="build"
TEST_EXECUTABLE="runTests"
RUN_VALGRIND=0
LIST_TESTS=0
FILTER="*"
REPEAT=1
SHUFFLE=0
BREAK_ON_FAILURE=0
TIMEOUT=0
XML_OUTPUT=0
PARALLEL_TESTS=1
USE_COLOR="auto"

# Help message
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS]

Test options:
    --list                List all available tests
    --filter PATTERN     Only run tests matching the pattern (default: "*")
    --repeat N           Repeat tests N times (default: 1)
    --shuffle           Randomize test execution order
    --break-on-failure  Stop on first test failure
    --valgrind          Run tests with valgrind
    --build-dir DIR     Specify build directory (default: "build")
    --verbose           Show detailed output
    --help              Show this help message
    --timeout N         Set test timeout in seconds
    --xml-output        Generate XML test results
    --parallel N        Run N tests in parallel
    --color             Force colored output
    --no-color          Disable colored output

Examples:
    $(basename "$0")                         # Run all tests
    $(basename "$0") --filter="MyTest*"      # Run tests matching pattern
    $(basename "$0") --repeat=3 --shuffle    # Run tests 3 times in random order
    $(basename "$0") --list                  # List available tests
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
        --list)
            LIST_TESTS=1
            shift
            ;;
        --filter)
            FILTER="$2"
            shift 2
            ;;
        --repeat)
            REPEAT="$2"
            shift 2
            ;;
        --shuffle)
            SHUFFLE=1
            shift
            ;;
        --break-on-failure)
            BREAK_ON_FAILURE=1
            shift
            ;;
        --valgrind)
            RUN_VALGRIND=1
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
        --timeout)
            TIMEOUT="$2"
            shift 2
            ;;
        --xml-output)
            XML_OUTPUT=1
            shift
            ;;
        --parallel)
            PARALLEL_TESTS="$2"
            shift 2
            ;;
        --color)
            USE_COLOR="yes"
            shift
            ;;
        --no-color)
            USE_COLOR="no"
            shift
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
log "Changing to build directory '$BUILD_DIR'..."
cd "$BUILD_DIR"

# Check if test executable exists
if [ ! -f "$TEST_EXECUTABLE" ]; then
    echo "Error: Test executable '$TEST_EXECUTABLE' not found in $BUILD_DIR"
    echo "Please build the tests first"
    exit 1
fi

# Make sure executable is executable
if [ ! -x "$TEST_EXECUTABLE" ]; then
    log "Making test executable '$TEST_EXECUTABLE' executable..."
    chmod +x "$TEST_EXECUTABLE"
fi

# Construct the test command with GTest options
CMD="./$TEST_EXECUTABLE"
CMD="$CMD --gtest_filter=$FILTER"
[ $REPEAT -gt 1 ] && CMD="$CMD --gtest_repeat=$REPEAT"
[ $SHUFFLE -eq 1 ] && CMD="$CMD --gtest_shuffle"
[ $BREAK_ON_FAILURE -eq 1 ] && CMD="$CMD --gtest_break_on_failure"
[ $LIST_TESTS -eq 1 ] && CMD="$CMD --gtest_list_tests"

# Add valgrind if requested
if [ $RUN_VALGRIND -eq 1 ]; then
    if ! command -v valgrind >/dev/null 2>&1; then
        echo "Error: valgrind not found"
        echo "Please install valgrind to use --valgrind option"
        exit 1
    fi
    CMD="valgrind --leak-check=full --show-leak-kinds=all $CMD"
fi

# Add XML output support
if [ $XML_OUTPUT -eq 1 ]; then
    CMD="$CMD --gtest_output=xml:test_results.xml"
fi

# Add timeout support
if [ $TIMEOUT -gt 0 ]; then
    CMD="timeout $TIMEOUT $CMD"
fi

# Add parallel test execution
if [ $PARALLEL_TESTS -gt 1 ]; then
    CMD="$CMD --gtest_parallel=$PARALLEL_TESTS"
fi

# Add color control
case $USE_COLOR in
    "yes")
        CMD="$CMD --gtest_color=yes"
        ;;
    "no")
        CMD="$CMD --gtest_color=no"
        ;;
esac

# Run the tests
log "Running command: $CMD"
echo -e "\n=== Test Execution ==="
eval "$CMD"
EXIT_CODE=$?

# Handle exit code
if [ $EXIT_CODE -eq 0 ]; then
    log "All tests passed successfully"
else
    echo "Tests failed with exit code $EXIT_CODE"
    exit $EXIT_CODE
fi