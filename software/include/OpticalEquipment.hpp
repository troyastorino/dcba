#ifndef OPTICAL_EQUIPMENT_H
#define OPTICAL_EQUIPMENT_H

#include <opencv2/core/core.hpp>
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
  OpticalEquipment();
};

class Camera : OpticalEquipment {
    
};

enum ProjectorType {
  DLP,
  STATIC_LED
};

class Projector : OpticalEquipment {
protected:
  ProjectorType type;
};


#endif
