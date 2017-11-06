# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 2.8)

add_definitions(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

# Set the project name
project (treesearch)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/utils.cpp
    src/BPlusTree.cpp
    src/main.cpp
)

# Add an executable with the above sources
add_executable(treesearch ${SOURCES})
