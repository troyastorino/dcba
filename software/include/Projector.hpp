#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "OpticalEquipment.hpp"

class Projector : OpticalEquipment {
public:
  enum ProjectorType {
    DLP,
    STATIC_LED
  };

protected:
  ProjectorType type;
};

#endif
