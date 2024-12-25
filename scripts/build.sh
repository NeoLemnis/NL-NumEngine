# Check if the build directory exists, if not create it
if [ ! -d "build" ]; then
  echo "Creating build directory ..."
  mkdir build
  echo -e "Build directory created\n"
fi

# Change to the build directory
echo "Changing to build directory ..."
cd build
echo -e "Changed to build directory\n"

# If cmake .. fails, exit the script
# Export compile commands for clangd
echo "Running cmake ..."
if ! cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..; then
  exit 1
fi
echo -e "Cmake ran successfully\n"

# If compile_commands.json exists in the root directory, delete it
echo "Moving compile_commands.json to root directory ..."
if [ -f "../compile_commands.json" ]; then
  rm ../compile_commands.json
fi
# Move compile_commands.json to the root directory
mv compile_commands.json .. 
echo -e "Moved compile_commands.json to root directory\n"

# If make fails, exit the script
echo "Running make ..."
if ! make; then
  exit 1
fi
echo -e "Make ran successfully\n"

# If the executable does not exist, exit the script
echo "Checking for executable ..."
if [ ! -f "NL-NumEngine" ]; then
  exit 1
fi
echo -e "Executable found\n"

echo -e "Running the executable ...\n"

# Run the executable
./NL-NumEngine