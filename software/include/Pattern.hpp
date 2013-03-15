#ifndef PATTERN_H
#define PATTERN_H

#include "Projector.hpp"
#include <opencv2/core/core.hpp>
#include "vecmath.h"

class Pattern {
protected:
  const Projector::ProjectorType type;
  Pattern(const Projector::ProjectorType type) : type(type) {};
};

class StaticPattern : Pattern {
public:
  StaticPattern() : Pattern(Projector::STATIC_LED) {};
};

class DLPPattern : Pattern {
protected:
  const cv::Mat& image;
public:
  DLPPattern(const cv::Mat& img) : Pattern(Projector::DLP), image(img) {};
  ~DLPPattern();
};

class GeneratedPattern {
protected:
  const cv::Mat& image;
  const vector<Projector*> projectors;
public:
  GeneratedPattern(const cv::Mat& img, const vector<Projector*> p) : image(img), projectors(p) {};
};

#endif
