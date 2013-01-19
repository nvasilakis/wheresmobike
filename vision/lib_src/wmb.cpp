
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

static void makeItSmall(const MatColor &img, MatColor &small)
{
  const int MAX_ROWS = 160;
  const int NOMINAL_COLS = 160;

  const double fx = double(NOMINAL_COLS)/img.cols;
  const int rows = roundToNearestInt(img.rows * fx);
  if(rows <= MAX_ROWS) {
    resize(img, small, Size(NOMINAL_COLS, rows));
  } else {
    double fy = double(MAX_ROWS)/img.rows;
    int cols = roundToNearestInt(img.cols * fy);
    CV_DbgAssert(cols <= NOMINAL_COLS);
    resize(img, small, Size(cols, MAX_ROWS));
  }

  DEBUG(small.rows);
  DEBUG(small.cols);
}

static void reviseCircleEstimates(Circles &circles)
{

}

static bool sortByRadius(const Vec3f &a, const Vec3f &b)
{
  return a[2] < b[2];
}

static constexpr bool isOdd(const int i) { return i&1; }
static constexpr bool isEven(const int i) { return !isOdd(i); }

static double findMedianWheelRadius(Circles & wheels)
{
  DEBUG(wheels.size());
  auto b = wheels.begin();
  auto m = b+wheels.size()/2;
  auto e = wheels.end();

  nth_element(b, m, e, &sortByRadius);
  double median = m[0][2];

  if(isEven(wheels.size())) {
    nth_element(b, m-1, m, &sortByRadius);
    median += m[1][2];
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

  wheelRadius_ = findMedianWheelRadius(allCircles_);
  DEBUG(wheelRadius_);

  return true;
}

void WmbVision::proprocessImage(const MatColor& img)
{
  // preprocess image
  makeItSmall(img, small_);
  cvtColor(small_, smallGray_, CV_BGR2GRAY);
  GaussianBlur(smallGray_, smallGray_, Size(5, 5), 0.0, 0.0);
  Canny(smallGray_, canny_, cannyThreshHigh_, cannyThreshLow_);
}

bool WmbVision::process(const MatColor & img)
{
  // preprocess image
  proprocessImage(img);
  // find wheels
  if(!findWheels()) return false;

  // find fork
  const int cols = canny_.cols;
  HoughLinesP(canny_, lines_,
      linesDistanceResolution(), linesAngleResolution(),
      linesAccumThresh(wheelRadius_), minLineLength(wheelRadius_),
      maxLineGap(wheelRadius_));

#ifndef NDEBUG
  imshow("img", img);
  imshow("smallBlurred", smallGray_);
  imshow("canny", canny_);
  displayCircles(smallGray_, allCircles_, "all");
  displayLines(smallGray_, lines_, "all");
  waitKey(0);
#endif

  return false;
}

uint8_t WmbVision::getDominantHue() const
{
  WARN_STR("STUB");
  return 0;
}

} // namespace wmb
