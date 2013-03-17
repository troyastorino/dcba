#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


vector<Point3f> generateCornerCoordinates(Size size, double sideLength) {
  vector<Point3f> points;
  for (int i = 0; i < size.width; i++) {
    for (int j = 0; j < size.height; j++) {
      points.push_back(Point3f(i*sideLength, j*sideLength, 0));
    }
  }
  return points;
}

Mat getCalibrationData(const Image& img, bool subPixel = false) {
  Size interiorCorners(6,8); // the interior corners of the chessboard image
  vector<Point2f> corners;

  // list of the points in the global frame
  vector<Point3f> cornersGlobalFrame = generateCornerCoordinates(interiorCorners, 1.8); 

  bool patternFound = findChessboardCorners(img.data, interiorCorners, corners,
                                            CV_CALIB_CB_ADAPTIVE_THRESH |
                                            CV_CALIB_CB_FAST_CHECK |
                                            CV_CALIB_CB_NORMALIZE_IMAGE);

  if (subPixel) {
    // right now this seems to give an unfortunate error, so making it not callable
    throw("Not implemented properly.  Gives an opencv error.  Disable this line if you want to see it.");

    // create a gray image for subpixel recognition
    Mat gray;
    cvtColor(img.data, gray, CV_BGR2GRAY);

    cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                  TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  }

  drawChessboardCorners(img.data, interiorCorners, Mat(corners), patternFound);

  // return value initialization
  Mat cameraMatrix;  

  // if we do have corners, perform calibration  
  if (patternFound) {
    // initialize outputs of the calibration function
    cameraMatrix = Mat::eye(3,3,CV_64F);
    Mat distCoefficients = Mat::zeros(8, 1, CV_64F);
    vector<Mat> rvecs, tvecs;
    
    // get the inputs for the calibtration function
    vector<vector<Point3f> > objectPoints;
    objectPoints.push_back(generateCornerCoordinates(interiorCorners, 1.8));
    vector<vector<Point2f> > imagePoints;
    imagePoints.push_back(corners);
    Size imageSize(img.data.cols, img.data.rows);

    // run the calibration
    calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoefficients, rvecs, tvecs);
  }
  
  return cameraMatrix;
}

int main(int n, char** args) {
  
  // Note: for some reason my computer doesn't like using device 1 and 2 at the
  // same time...1 is the webcam
  Camera cam0(VideoCapture(0));
  Camera cam1(VideoCapture(2));

  // Create windows for images
  namedWindow("0", CV_WINDOW_AUTOSIZE);
  namedWindow("1", CV_WINDOW_AUTOSIZE);

  while (true) {
    // capture frames
    Image frame0 = Camera::captureImage(cam0);
    Image frame1 = Camera::captureImage(cam1);

    // get calibration data
    getCalibrationData(frame0);
    getCalibrationData(frame1);

    // show the images
    imshow("0", frame0.data);
    imshow("1", frame1.data);

    // wait 10 ms for a key to be pressed
    if (waitKey(10) > 0) break;
  }

  return 0;
}
