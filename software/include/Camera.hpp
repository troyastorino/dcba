#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include "OpticalEquipment.hpp"
#include "Image.hpp"

class Image;

class Camera : OpticalEquipment {
  // for other initialization possibilities, see http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html?highlight=capturefromcam#videocapture-videocapture
  static Image *captureImage(const int deviceNum = 0);
  static Image *captureImage(const cv::VideoCapture* capture);
  static std::vector<Image> captureImages(int deviceNum = 0, int numImages = 1, int delayMillis = 10);
};

#endif
