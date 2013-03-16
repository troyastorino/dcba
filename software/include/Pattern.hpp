#ifndef PATTERN_H
#define PATTERN_H

#include "Projector.hpp"
#include <opencv2/core/core.hpp>
#include "vecmath.h"

/*
 * Class: Pattern
 * This functions as a superclass of specific pattern implementations. It has a
 * type, but no other information. Subclasses must contain sufficient
 * information to define the specific projected image.
 *
 * Superclass of <StaticPattern>, <DLPPattern>, and <GeneratedPattern>
 *
 * Properties:
 * type - the type of projector that can create this pattern
 */

class Pattern {
protected:
  const Projector::ProjectorType type;
  Pattern(const Projector::ProjectorType type) : type(type) {};
};

/* Class: StaticPattern
 * A pattern class to be used for the turning on and off of static projectors,
 * most likely implemented as a grid over an LED. This pattern has no
 * information other than type, as static projectors can only create a single
 * pattern.
 */

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
