cmake_minimum_required(VERSION 3.10)
project(ContactManagement VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(ContactManagement main.cpp ${SOURCES})

# Include directories
target_include_directories(ContactManagement PRIVATE include)