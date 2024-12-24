# Check if the build directory exists, if not create it
if [ ! -d "build" ]; then
  mkdir build
fi

# Change to the build directory
cd build

# If cmake .. fails, exit the script
if ! cmake ..; then
  exit 1
fi

# If make fails, exit the script
if ! make; then
  exit 1
fi

# If the executable does not exist, exit the script
if [ ! -f "NL-NumEngine" ]; then
  exit 1
fi

# Run the executable
./NL-NumEngine