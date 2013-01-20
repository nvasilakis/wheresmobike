
#include <algorithm>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "wmb/wmb.h"
#include "wmb/logging.h"
#include "wmb/round.h"
#include "wmb/wmb_params.h"

#ifndef NDEBUG
#include "wmb/viz.h"
#endif

using namespace std;
using namespace cv;

namespace wmb
{

typedef bool (*circle_sort_fn)(const Vec3f &, const Vec3f &);

template <int N>
static bool sortBy(const Vec3f &a, const Vec3f &b)
  { return a[N] < b[N]; }

static constexpr bool isOdd(const int i) { return i&1; }
static constexpr bool isEven(const int i) { return !isOdd(i); }

template <int N>
static double findMedianWheel(Circles & wheels)
{
  DEBUG(wheels.size());
  const auto b = wheels.begin();
  const auto m = b+wheels.size()/2;
  const auto e = wheels.end();

  nth_element(b, m, e, sortBy<N>);
  double median = m[0][N];

  if(isEven(wheels.size())) {
    nth_element(b, m-1, m, sortBy<N>);
    median += m[-1][N];
    median *= 0.5;
  }

  return median;
}

bool WmbVision::findWheels()
{
  // hough transform params
  const int cols = smallGray_.cols;

  // hough circle transform on each half to find front/back wheels
  HoughCircles(smallGray_.colRange(0, cols / 2), circlesL_, CV_HOUGH_GRADIENT,
      houghCircleShrinkFactor(), minWheelDist(cols), cannyThreshHigh_,
      circleAccumThresh(cols), minWheelRadius(cols), maxWheelRadius(cols));
  HoughCircles(smallGray_.colRange(cols / 2, cols), circlesR_, CV_HOUGH_GRADIENT,
      houghCircleShrinkFactor(), minWheelDist(cols), cannyThreshHigh_,
      circleAccumThresh(cols), minWheelRadius(cols), maxWheelRadius(cols));
  DEBUG(circlesL_.size());
  DEBUG(circlesR_.size());
  if (circlesL_.empty() || circlesR_.empty())
  {
    WARN_STR("no circles found");
    return false;
  }

  // merge halves
  allCircles_ = circlesL_;
  allCircles_.reserve(circlesL_.size() + circlesR_.size());
  for (Vec3f& circle : circlesR_)
  {
    allCircles_.emplace_back(circle[0] + cols / 2, circle[1], circle[2]);
  }

  assert(wheels_.size() == 2);
  wheelRadius_ = findMedianWheel<2>(allCircles_);
  wheels_[0] = Vec3f(findMedianWheel<0>(circlesL_),
                     findMedianWheel<1>(circlesL_),
                     wheelRadius_);
  wheels_[1] = Vec3f(findMedianWheel<0>(circlesR_) + cols/2,
                     findMedianWheel<1>(circlesR_),
                     wheelRadius_);
  DEBUG(wheelRadius_);

  return true;
}

} // namespace wmb
