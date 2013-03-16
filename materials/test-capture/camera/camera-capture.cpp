#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// A Simple Camera Capture Framework 
int main() {
  VideoCapture capture0(0);
  VideoCapture capture1(2);
  
  // Create a window in which the captured images will be presented
  namedWindow( "0", CV_WINDOW_AUTOSIZE );
  namedWindow( "1", CV_WINDOW_AUTOSIZE );
  // Show the image captured from the camera in the window and repeat
  while ( true ) {
    // Get one frame
    Mat frame0, frame1, frame2;
    
    capture0 >> frame0;
    capture1 >> frame1;

    // show the image
    imshow( "0", frame0 );
    imshow( "1", frame1 );
    
    // wait for 10 ms for a key to be pressed; if one is, exit
    if ( waitKey(10) > 0) break;
  }

  return 0;
}
