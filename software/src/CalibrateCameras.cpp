#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Generates board coordinates of all the interior corners on the checkerboard
vector<Point3f> generateCornerCoordinates(Size size, float sideLength) {
  vector<Point3f> points;
  for (int i = 0; i < size.width; i++) {
    for (int j = 0; j < size.height; j++) {
      points.push_back(Point3f(i*sideLength, j*sideLength, 0));
    }
  }
  return points;
}

// interiorCorners: the dimensions of the interior corners of the chessboard
// squareSideLength: the side length of a checkerboard square
// returns a vector of image points representing the board corners if a pattern
// is found, or else an empty vector if no pattern is found
vector<Point2f> findCheckerboardCorners(const Image& img, Size interiorCorners,
                                        float squareSideLength, bool subPixel = false) {
  // initialize vector of chessboard corner locations
  vector<Point2f> imageCorners;

  // look for the checkerboard pattern
  bool patternFound = findChessboardCorners(img.data, interiorCorners, imageCorners,
                       CV_CALIB_CB_ADAPTIVE_THRESH |
                       CV_CALIB_CB_FAST_CHECK |
                       CV_CALIB_CB_NORMALIZE_IMAGE);
  
  // if subPixel accuracy is desired, perform that calculation
  if (subPixel) {
    // right now this seems to give an unfortunate error, so making it not callable
    throw("Not implemented properly.  Gives an opencv error.  Disable this line if you want to see it.");

    // create a gray image for subpixel recognition
    Mat gray;
    cvtColor(img.data, gray, CV_BGR2GRAY);

    cornerSubPix(gray, imageCorners, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  }

  // draw the chessboard corners on the image
  drawChessboardCorners(img.data, interiorCorners, Mat(imageCorners), patternFound);

  // return the image corners
  if (patternFound) {
    return imageCorners;
  } else {
    return vector<Point2f>();
  }
}

/* calibrateCameraWithCheckerboard: Captures a sequence of images in order to
 * calibrate a camera. You should be holding the checkerboad calibration sheet
 * at different angles while the camera is calibrating
 * TODO: abstract away to running the program over a series of images instead of
 * just for capturing images
 *
 * Size interiorCorners: the dimensions of the interior corners of the
 * checkerboard
 * float squareSideLength: the side length of one of the squares of the
 * checkerboard
 * VideoCapture capture: the videocapture device that will be taking the images
 * int numImages: the number of images desired for the capture sequence
 * int delayTime: the time to have the program wait between captured images to
 * allow you to move the checkerboard (in seconds)
 */
Camera calibrateCameraWithCheckerboard(Size interiorCorners, float squareSideLength,
                                       VideoCapture capture, int numImages = 5, int delayTime = 2) {
  // name of the window
  string name = "Camera Calibration";
  
  // open a window to view the images
  namedWindow(name, CV_WINDOW_AUTOSIZE);

  // initialize a vector of vectors of image points
  vector<vector<Point2f> > imagePoints;

  // initialize a vector of vectors of checkerboard points
  vector<vector<Point3f> > boardPoints;

  // list of the points in the global frame
  vector<Point3f> boardCorners = generateCornerCoordinates(interiorCorners, squareSideLength);

  // initialize image size on first image
  bool imageSizeInitialized = false;
  Size imageSize;
  while (imagePoints.size() < 10) {
    // capture an image
    Image img = Camera::captureImage(capture);

    // if imageSize doesn't have a value initialize it
    if (!imageSizeInitialized) {
      imageSize = Size(img.data.cols, img.data.rows);
    }

    // find the checkerboard corners
    vector<Point2f> imageCorners = findCheckerboardCorners(img, interiorCorners, squareSideLength);

    // show the image
    imshow(name, img.data);
    
    if (imageCorners.size() > 0) {
      // add the coordinates from the image
      imagePoints.push_back(imageCorners);
      boardPoints.push_back(boardCorners);

      // sleep so the user can move the image to a new angle (in microseconds)
      waitKey(delayTime * 1000);
    }

    waitKey(10);
  }

  // initialize return values from calibration procedure
  Mat cameraMatrix = Mat::eye(3,3,CV_64F);
  Mat distCoefficients = Mat::zeros(8, 1, CV_64F);
  vector<Mat> rvecs, tvecs;

  // run the calibration
  calibrateCamera(boardPoints, imagePoints, imageSize, cameraMatrix, distCoefficients, rvecs, tvecs);

  Camera cam(capture, cameraMatrix, distCoefficients);

  return cam;
}

int main(int n, char** args) {
  // the interior corners of the checkerboard image
  Size interiorCorners(6,8);

  // sideLength of the checkerboard square 
  float sideLength = 1.8;

  // initialize both cameras (numbers may be different on different machines
  VideoCapture capture0(0);
  VideoCapture capture1(2);

  // calibrate the two cameras
  Camera cam0 = calibrateCameraWithCheckerboard(interiorCorners, sideLength, capture0);
  Camera cam1 = calibrateCameraWithCheckerboard(interiorCorners, sideLength, capture1);
  
  return 0;
}
