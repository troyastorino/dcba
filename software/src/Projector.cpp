#include "Projector.hpp"
#include <opencv2/core/core.hpp>

using namespace std;

GeneratedPattern DLPProjector::projectPattern( const DLPProjector& projector, const DLPPattern& pattern ) {
  // creates a new matrix and distorts the pattern according to the projector
  // parameters
  cv::Mat& image = pattern.image;

  // wraps that new image in a GeneratedPattern and returns it
}
