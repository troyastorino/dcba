add_executable(compile_test CompileTest.cpp Camera.cpp)
target_link_libraries(compile_test ${OpenCV_LIBS} VecMath)

