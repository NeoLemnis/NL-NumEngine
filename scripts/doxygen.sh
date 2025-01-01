#!/bin/bash
# Generate and open documentation using Doxygen
#
# This script:
# 1. Generates documentation using Doxygen
# 2. Opens the generated documentation in the default web browser
# 3. Includes error handling and status checks

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen is not installed"
    echo "Please install it using your package manager"
    exit 1
fi

# Check if Doxyfile exists
if [ ! -f "Doxyfile" ]; then
    echo "Error: Doxyfile not found in current directory"
    exit 1
fi

# Generate documentation
echo "Generating documentation..."
if ! doxygen Doxyfile; then
    echo "Error: Documentation generation failed"
    exit 1
fi

# Check if documentation was generated
if [ ! -d "html" ] || [ ! -f "html/index.html" ]; then
    echo "Error: Documentation files were not generated properly"
    exit 1
fi

# Open documentation in default browser
echo "Opening documentation in browser..."
xdg-open html/index.html > /dev/null 2>&1 &

echo "Documentation generated successfully!"
