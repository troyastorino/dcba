#ifndef IMAGE_H
#define IMAGE_H

#include "OpticalEquipment.hpp"
#include "Pattern.hpp"
#include <opencv2/core/core.hpp>


class Image {
protected:
  // The pattern that was projected when the image was captured
  const GeneratedPattern * pattern;
        
  // The camera that captured the image
  const Camera * camera;
        
  // The image data
  const cv::Mat image;
};

#endif
