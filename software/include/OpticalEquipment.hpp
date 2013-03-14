#ifndef OPTICAL_EQUIPMENT_H
#define OPTICAL_EQUIPMENT_H

#include <opencv2/opencv.hpp>
#include "vecmath.h"

using namespace std;

class OpticalEquipment {
protected:
  // Extrinsic parameters //
  // Orientation and location of the camera in world coordinates
  const Matrix4f pose;

  // Intrinsic Parameters //
  // Focal length
  const float focalLength;

  // Camera center, or principal point
  const Vector2f principalPoint;

  // Skew coefficient
  const float alpha;

  // Lens distortion coefficients -- is a vector whose length depends on the
  // order of the lens distortion model
  const vector<float> distortion;

public:
  // Constructor: Default constructor for OpticalEquipment that puts dummy
  // values for all of the variables. TODO: remove this constructor once we have
  // a system that can get real variables
  OpticalEquipment():
    pose(Matrix4f()), focalLength(1), principalPoint(Vector2f(0,0)),
    alpha(0), distortion(vector<float>()) {};
};
#endif
