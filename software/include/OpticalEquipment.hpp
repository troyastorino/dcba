#ifndef OPTICAL_EQUIPMENT_H
#define OPTICAL_EQUIPMENT_H

#include <opencv2/opencv.hpp>
#include "vecmath.h"

using namespace std;
using namespace cv;

/*
 * Class: OpticalEquipment
 * Generic superclass for all cameras and projectors used in the set up. Cameras
 * and projectors can both be represented as an imaging surface and a reference
 * point. This class contains the information defining those two features and
 * their calibration.
 *
 * Superclass of <Projector> and <Camera>
 *
 * Properties:
 * pose - 4x4 matrix defining position and orientation of equipment in space
 * focalLength - the focal length of the device
 * principalPoint - the position at which a ray from the center of the light
 * emitter (reference point) intersects the imaging plane
 * alpha - the skew of the imaging plane, as the plane is not guaranteed to be a
 * perfect rectangle
 * distortion - the lens distortion coefficient. The number of values defines
 * the order of the distortion model
 */

class OpticalEquipment {
  public:
    // Extrinsic parameters //
    // Orientation and location of the camera in world coordinates
    const Matrix4f pose;

    // Intrinsic Parameters //
    // Camera matrix
    const Mat intrinsicMatrix;

    // Lens distortion coefficients -- is a vector whose length depends on the
    // order of the lens distortion model
    const Mat distortion;

  public:
    // Constructor
    // Default constructor for OpticalEquipment that puts dummy
    // values for all of the variables. TODO: remove this constructor once we have
    // a system that can get real variables
    OpticalEquipment():
      pose(Matrix4f()), intrinsicMatrix(Mat()), distortion(Mat()) {};

    OpticalEquipment(Mat intrinsicMatrix, Mat distortion):
      pose(Matrix4f()), intrinsicMatrix(intrinsicMatrix), distortion(distortion) {};
};
#endif
