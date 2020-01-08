# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(targetName "render3d_app")
project(${targetName})

set(targetSource 
    "main.cpp"
    "Application.cpp"
    "Window.cpp"
    "Logic.cpp"
    "tinyrenderer/model.cpp"
    )

#include_directories()
add_executable(${targetName} ${targetSource})

target_link_libraries(${targetName} SDL2-static SDL2main)

add_custom_command(
        TARGET ${targetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/data/african_head.obj
                ${CMAKE_CURRENT_BINARY_DIR}/african_head.obj)
