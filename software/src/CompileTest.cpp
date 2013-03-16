#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int n, char** args) {
  cout << "Capturing image" << endl;
  
  // Note: for some reason my computer doesn't like using device 1 and 2 at the
  // same time...1 is the webcam
  Camera cam0(VideoCapture(0));
  Camera cam1(VideoCapture(2));  
  
  Image frame0 = Camera::captureImage(cam0);
  Image frame1 = Camera::captureImage(cam1);
  
  namedWindow("0", CV_WINDOW_AUTOSIZE);
  namedWindow("1", CV_WINDOW_AUTOSIZE);
  
  imshow("0", frame0.data);
  imshow("1", frame1.data);
  
  waitKey(0);

  return 0;
}
