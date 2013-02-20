project( Capture )
find_package( OpenCV REQUIRED )
find_package(OpenGL REQUIRED)
find_package(GLUT)
find_package(PNG)
add_executable( Capture capture )
target_link_libraries( Capture ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} )

