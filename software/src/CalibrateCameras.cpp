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
vector<Point2f> findCheckerboardCorners(Mat imageData, Size interiorCorners,
                                        float squareSideLength, bool subPixel = false) {
  // initialize vector of chessboard corner locations
  vector<Point2f> imageCorners;

  // look for the checkerboard pattern
  bool patternFound = findChessboardCorners(imageData, interiorCorners, imageCorners,
                       CV_CALIB_CB_ADAPTIVE_THRESH |
                       CV_CALIB_CB_FAST_CHECK |
                       CV_CALIB_CB_NORMALIZE_IMAGE);
  
  // if subPixel accuracy is desired, perform that calculation
  if (subPixel) {
    // right now this seems to give an unfortunate error, so making it not callable
    throw("Not implemented properly.  Gives an opencv error.  Disable this line if you want to see it.");

    // create a gray image for subpixel recognition
    Mat gray;
    cvtColor(imageData, gray, CV_BGR2GRAY);

    cornerSubPix(gray, imageCorners, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  }

  // draw the chessboard corners on the image
  drawChessboardCorners(imageData, interiorCorners, Mat(imageCorners), patternFound);

  // return the image corners
  if (patternFound) {
    return imageCorners;
  } else {
    return vector<Point2f>();
  }
}

/*
 * captureCheckerboardCalibrationImages: captures a sequence of images to be
 * used for calibrating a camera. Shows the images being produced by the camera,
 * and pauses if the image is a good one
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
vector<Image> captureCheckerboardCalibrationImages(VideoCapture capture, Size interiorCorners, float squareSideLength,
                                                   int numImages = 5, int delayTime = 2) {
  // name of the window to view images in
  string name = "Camera Calibration";
  
  // open a window to view the images in
  namedWindow(name, CV_WINDOW_AUTOSIZE);

  // initialize the structure to hold the return images
  vector<Image> images;
  
  // collect the images
  while (images.size() < numImages) {
    // capture an image
    Image img = Camera::captureImage(capture);
    // create a copy of the image data
    Mat copy = img.data.clone();
    
    vector<Point2f> corners = findCheckerboardCorners(copy, interiorCorners, squareSideLength);

    // show the image
    imshow(name, copy);
    waitKey(10); // necessary for opencv to do the window display

    // add the image if it has corners
    if (corners.size() > 0) {
      images.push_back(img);

      // sleep so the user can move the image to a new angle (in microseconds)
      waitKey(delayTime * 1000);
    }
  }

  return images;
}

/* calibrateCameraWithCheckerboard: From a series of checkerboard images
 * initializes a calibrated Camera object
 *
 * VideoCapture capture: the device that can capture images
 * vector<Image> images: the images of the checkerboard to be used for
 * calibration. This images must be from the same camera, and so must be the
 * same size
 * Size interiorCorners: the dimensions of the interior corners of the
 * checkerboard
 * float squareSideLength: the side length of one of the squares of the
 * checkerboard
 */
Camera calibrateCameraWithCheckerboard(VideoCapture capture, vector<Image> images, Size interiorCorners, float squareSideLength) {
  // initialize a vector of vectors of image points
  vector<vector<Point2f> > imagePoints;

  // initialize a vector of vectors of checkerboard points
  vector<vector<Point3f> > boardPoints;

  // list of the points in the global frame
  vector<Point3f> boardCorners = generateCornerCoordinates(interiorCorners, squareSideLength);

  // generate image and board points for all of the images in the function
  for (int i = 0; i < images.size(); i++) {
    vector<Point2f> corners = findCheckerboardCorners(images[i].data, interiorCorners, squareSideLength);
    if (corners.size() > 0) {
      imagePoints.push_back(corners);
      boardPoints.push_back(boardCorners);
    } else {
      stringstream ss;
      ss << "The image with index i = " << i << "is invalid" << endl;
      throw(ss.str());
    }
  }

  // initialize image size on first image
  Size imageSize(images[0].data.cols, images[0].data.rows);

  // initialize return values from calibration procedure
  Mat cameraMatrix = Mat::eye(3,3,CV_64F);
  Mat distCoefficients = Mat::zeros(8, 1, CV_64F);
  vector<Mat> rvecs, tvecs;

  // run the calibration
  calibrateCamera(boardPoints, imagePoints, imageSize, cameraMatrix, distCoefficients, rvecs, tvecs);

  // initialize the camera
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

  // capture images for both cameras
  vector<Image> images0 = captureCheckerboardCalibrationImages(capture0, interiorCorners, sideLength);
  vector<Image> images1 = captureCheckerboardCalibrationImages(capture1, interiorCorners, sideLength);
  
  // calibrate the two cameras
  Camera cam0 = calibrateCameraWithCheckerboard(capture0, images0, interiorCorners, sideLength);
  Camera cam1 = calibrateCameraWithCheckerboard(capture1, images1, interiorCorners, sideLength);
  
  return 0;
}
