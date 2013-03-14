#include "Camera.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

Image* Camera::captureImage(const int deviceNum) {
  VideoCapture capture(deviceNum);
  return Camera::captureImage(&capture);
}

Image* Camera::captureImage(const VideoCapture *capture) {
  // capture an image
  Mat frame;
  Camera cam(capture);
  VideoCapture cap = *capture;
  cap >> frame;

  // check if capture succeeded
  if (frame.empty()) {
    throw("ERROR: Image capture failed.");
  } else {
    Image img(&frame, &cam, vector<GeneratedPattern*>());
    return &img;
  }
}
