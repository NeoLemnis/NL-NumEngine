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

# Run the executable
./NL-NumEngine