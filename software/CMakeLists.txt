cmake_minimum_required(VERSION 2.8)

project(DCBA)

# build path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# include paths
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/vecmath/include)

# link paths
link_directories(${CMAKE_SOURCE_DIR}/include)

# add opencv
find_package(OpenCV REQUIRED)

# add directories
add_subdirectory(include)
add_subdirectory(src)

# add libraries
add_subdirectory(lib/vecmath)
