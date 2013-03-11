cmake_minimum_required(VERSION 2.8)

project(DCBA)

# build path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# include paths
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/vecmath/include)

# source directory
add_subdirectory(src)

# add opencv
find_package(OpenCV REQUIRED)
