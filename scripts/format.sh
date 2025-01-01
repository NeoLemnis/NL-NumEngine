 #!/bin/bash
# Project: NL-NumEngine
# Script: format.sh
#
# Description:
#   Formats source code using clang-format and optionally other formatters.
#
# Dependencies:
#   - clang-format
#   - cmake-format (optional)
#   - prettier (optional for markdown)

set -e

# Configuration
VERBOSE=0
FIX=0
JOBS=$(nproc)

# Format source files
find src include -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} +

# Format CMake files
if command -v cmake-format >/dev/null 2>&1; then
    find . -name "CMakeLists.txt" -exec cmake-format -i {} +
fi

# Format markdown files
if command -v prettier >/dev/null 2>&1; then
    find . -name "*.md" -exec prettier --write {} +
fi