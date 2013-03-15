#include "Camera.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

Image Camera::captureImage(const int deviceNum) {
  VideoCapture capture(deviceNum);
  return Camera::captureImage(capture);
}

Image Camera::captureImage(VideoCapture& capture) {
  Camera camera(capture);
  return Camera::captureImage(camera);
}

Image Camera::captureImage(const Camera& camera) {
  // capture an image
  Mat frame;
  camera.device >> frame;

  // check if capture succeeded
  if (frame.empty()) {
    throw("ERROR: Image capture failed.");
  } else {
    Image img(frame, camera, vector<GeneratedPattern*>());
    return img;
  }
}
