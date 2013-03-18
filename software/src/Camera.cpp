#include "Camera.hpp"
#include "Image.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

Image Camera::captureImage(VideoCapture capture) {
 // capture an image
  Camera cam(capture);
  return captureImage(cam);
}

Image Camera::captureImage(Camera& camera) {
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
