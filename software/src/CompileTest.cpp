#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int n, char** args) {
  Image* frame = Camera::captureImage(0);

  namedWindow("Capture", CV_WINDOW_AUTOSIZE);

  cv::imshow("Capture", *(frame->image));

  waitKey(0);

  return 0;
}
