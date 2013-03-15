#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int n, char** args) {
  cout << "Capturing image" << endl;
  
  Image frame = Camera::captureImage(0);

  int x = 1;

  namedWindow("Capture", CV_WINDOW_AUTOSIZE);

  imshow("Capture", frame.data);

  waitKey(0);

  return 0;
}
