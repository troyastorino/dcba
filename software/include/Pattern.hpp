#ifndef PATTERN_H
#define PATTERN_H

#include "OpticalEquipment.hpp"
#include <opencv2/core/core.hpp>
#include <vecmath.h>

class Pattern {
protected:
  const ProjectorType type;
private:
  Pattern();
};

class StaticPattern : Pattern {
public:
  StaticPattern() : type(ProjectorType::STATIC_LED);
};

class DLPPattern : Pattern {
protected:
  const cv::Mat* image;
public:
  DLPPattern(const cv::Mat* img) : type(ProjectorType::DLP), image(img);
  ~DLPPattern();
};

class GeneratedPattern : Pattern {
protected:
  const cv::Mat* image;
  const vector<Projector*> projectors;
public:
  GeneratedPattern(const cv::Mat* img, const vector<Projector*> p) : image(img), projectors(p);
};

#endif
