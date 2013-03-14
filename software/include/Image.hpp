#ifndef IMAGE_H
#define IMAGE_H

#include "Camera.hpp"
#include "Pattern.hpp"
#include <opencv2/opencv.hpp>

class Camera;

/*
 * Class: Image
 *
 * Stores all the information relevant to one captured image from the
 * environment. This includes the patterns projected on the object when the
 * image was taken, the camera that took the image, and the image data itself,
 * in matrix form.
 *
 * Properties:
 * Patterns - the light patterns projected on the object when the image was
 * taken
 * camera - the camera that took the image
 * image - the pixel data comprising the image
 */
class Image {
  public:
    // Property: patterns
    // The pattern that was projected when the image was captured
    const vector<GeneratedPattern*> patterns;

    // Property: camera
    // The camera that captured the image
    const Camera * camera;

    // Property: image
    // The image data
    const cv::Mat * image;

    //
    Image(const cv::Mat * image, const Camera* camera, const vector<GeneratedPattern*> patterns):
      image(image), camera(camera), patterns(patterns) {};
};

#endif
