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

# Add Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the tests recursively from the tests directory
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Create the test executable
add_executable(runTests ${TEST_SOURCES})

target_include_directories(runTests PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Link the test executable with Google Test and the main library
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
add_test(NAME runTests COMMAND runTests)
