cmake_minimum_required(VERSION 2.8)
project(camera)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
add_executable(camera camera-capture.cpp)
target_link_libraries(camera ${OpenCV_LIBS})
