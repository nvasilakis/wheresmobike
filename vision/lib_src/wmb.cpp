
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "wmb/wmb.h"
#include "wmb/logging.h"
#include "wmb/round.h"

#ifndef NDEBUG
#include "wmb/viz.h"
#endif

using namespace cv;

namespace wmb
{

static void makeItSmall(const MatColor &img, MatColor &small)
{
  const int NOMINAL_ROWS = 120;
  const int NOMINAL_COLS = 160;

  const double fx = double(NOMINAL_COLS)/img.cols;
  const int rows = roundToNearestInt(img.rows * fx);
  if(rows <= NOMINAL_ROWS) {
    resize(img, small, Size(NOMINAL_COLS, rows));
  } else {
    double fy = double(NOMINAL_ROWS)/img.rows;
    int cols = roundToNearestInt(img.cols * fy);
    CV_DbgAssert(cols <= NOMINAL_COLS);
    resize(img, small, Size(cols, NOMINAL_ROWS));
  }

  DEBUG(small.rows);
  DEBUG(small.cols);
}

bool WmbVision::process(const MatColor & img)
{
  makeItSmall(img, small_);
  cvtColor(small_, smallGray_, CV_BGR2GRAY);
  GaussianBlur(smallGray_, smallGray_, Size(5, 5), 1.0, 1.0);
  Canny(smallGray_, canny_, cannyThreshHigh_, cannyThreshLow_);

  const int cols = smallGray_.cols;
  double dp = 1.0; // For example, if dp=1 , the accumulator has the same resolution as the input image. If dp=2 , the accumulator has half as big width and height.
  double minDist = cols/6.0;
  double param1=cannyThreshHigh_;
  double param2=cols/6.0; // accumulator threshold for the circle centers at the detection stage
  int minRadius=cols/8;
  int maxRadius=cols/3;

  HoughCircles(smallGray_.colRange(0, cols/2), circles_, CV_HOUGH_GRADIENT,
      dp, minDist, param1, param2, minRadius, maxRadius);
  HoughCircles(smallGray_.colRange(cols/2, cols), circlesR_, CV_HOUGH_GRADIENT,
      dp, minDist, param1, param2, minRadius, maxRadius);
  DEBUG(circles_.size());
  DEBUG(circlesR_.size());
  if(circles_.empty() || circlesR_.empty()) {
    return false;
  }
  circles_.reserve(circles_.size() + circlesR_.size());
  for(Vec3f &circle : circlesR_) {
    circles_.emplace_back(circle[0]+cols/2, circle[1], circle[2]);
  }

#ifndef NDEBUG
  imshow("smallBlurred", smallGray_);
  imshow("canny", canny_);
  displayCircles(smallGray_, circles_);
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
