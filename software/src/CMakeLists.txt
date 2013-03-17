add_executable(calibrate_cameras CalibrateCameras.cpp Camera.cpp)
target_link_libraries(calibrate_cameras ${OpenCV_LIBS} VecMath)

