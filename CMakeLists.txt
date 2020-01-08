# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("render3d")

# Include sub-projects.
include_directories(${CMAKE_SOURCE_DIR})
include_directories("ThirdParty")

add_subdirectory ("ThirdParty/SDL2")
add_subdirectory ("Render3d")
