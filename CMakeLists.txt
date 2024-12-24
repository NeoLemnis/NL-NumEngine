cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(NL-NumEngine VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the sources recursively and the main file
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "main.cpp")

# Create the executable target
add_executable(NL-NumEngine ${SOURCES})

# Include directories
target_include_directories(NL-NumEngine PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
