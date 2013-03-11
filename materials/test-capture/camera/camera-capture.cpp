#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// A Simple Camera Capture Framework 
int main() {
  VideoCapture capture(0);
  if ( !capture.isOpened() ) {
    cout << "ERROR: could not connect to an image capture device" << endl; 
    return -1;
  }
  
  // Create a window in which the captured images will be presented
  namedWindow( "Captured Video", CV_WINDOW_AUTOSIZE );
  
  // Show the image captured from the camera in the window and repeat
  while ( true ) {
    // Get one frame
    Mat frame;
    capture >> frame;
    if (frame.empty()) {
      cout << "ERROR: Image capture failed." << endl;
      return -1;
    }

    // show the image
    imshow( "Captured Video", frame );

    // wait for 10 ms for a key to be pressed; if one is, exit
    if ( waitKey(10) > 0) break;
  }

  return 0;
}
