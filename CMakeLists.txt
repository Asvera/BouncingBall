cmake_minimum_required(VERSION 3.28.3)

# Project name and version
project(BouncingBall VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)

# Find SDL2_image
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} -lSDL2 -lSDL2_ttf -lSDL2_image)

