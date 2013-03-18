#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include "OpticalEquipment.hpp"
#include "Image.hpp"

class Image;

/*
 * Class: Camera
 * Contains all information relevant to a physical camera. This includes a
 * reference to the video capture device itself and the information necessary to
 * convert images from the world space to camera space that result from calibration.
 *
 * Subclass of <OpticalEquipment>
 *
 * Properties:
 * device - a reference to the VideoCapture device defined by OpenCV that can be used to campture
 * an image
 */
class Camera : OpticalEquipment {
  public:
    //Property: OpenCV's VideoCapture object. TODO: May not want this as an
    //object property once we integrate our own cameras
    cv::VideoCapture device;

    // Constructor: Only takes a VideoCapture device as argument. TODO: remove
    // this constructor so that imaging devices will always have their intrinsic
    // and external parameters in their object
    Camera(cv::VideoCapture device) : device(device) {};
    Camera(cv::VideoCapture device, Mat cameraMatrix, Mat distortion) :
      device(device), OpticalEquipment(cameraMatrix, distortion) {};

        
    /*
     * Method: captureImage
     * Given the device location information for a camera, this orders that
     * camera to take a picture, and then captures the information in that
     * picture and returns it as an <Image> object
     */
    static Image captureImage(Camera& camera);
    static Image captureImage(cv::VideoCapture device);

  
    /*
     * Method: captureImages
     * Given the device location information for a camera, this orders that
     * camera to take a series of images with a certain delay, and returns a
     * vector of the captured images
     */
    static std::vector<Image> captureImages(int deviceNum = 0, int numImages = 1, int delayMillis = 10);

    /*
     * Method calculateCalibration
     * Assumes we are using a checkerboard to calibrate the camera
     */
};

#endif
