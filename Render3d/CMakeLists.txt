# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(targetName "render3d_app")
project(${targetName})

set(targetSource "main.cpp")
add_executable(${targetName} ${targetSource})

target_link_libraries(${targetName} SDL2-static SDL2main)
