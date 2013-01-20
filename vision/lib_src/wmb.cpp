
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

bool WmbVision::process(const MatColor & img)
{
  // preprocess image
  proprocessImage(img);
  // find wheels
  if(!findWheels()) return false;
  // find fork
  if(!findForks()) return false;
//  findForks();
  buildColorHistograms(img);

#ifndef NDEBUG
  imshow("img", img);
  imshow("smallBlurred", smallGray_);
  imshow("canny", canny_);
  displayCircles(smallGray_, allCircles_, "all");
  displayCircles(smallGray_, Circles({wheelL_, wheelR_}), "wheels");
  displayLines(canny_, allLines_, "all");
  displayLines(canny_, linesL_, "left");
  displayLines(canny_, linesR_, "right");
  displayLineSegments(smallGray_, {forkL_, forkR_}, "fork");
  displayColorHistograms(hist_);
  waitKey(0);
#endif

  return true;
}

MatFeatures WmbVision::getFeatures() const
{
  MatFeatures res = hist_.reshape(1, hist_.total());

  float x1 = wheelL_[0];
  float y1 = wheelL_[1];
  float x2 = wheelR_[0];
  float y2 = wheelR_[1];

  float dist = sqrt((x1 - x2) * (x1 - x2) +
                    (y1 - y2) * (y1 - y2));

  res.push_back(wheelRadius_ / dist);

  return res.reshape(1, 1);

}

static void drawCircle(MatColor &rgb, const Vec3f &ctrSz, const Scalar &color)
{
  Point c(roundToNearestInt(ctrSz[0]*16.0f), roundToNearestInt(ctrSz[1]*16.0f));
  int r = roundToNearestInt(ctrSz[2]*16.0f);
  circle(rgb, c, r, color, 1, CV_AA, 4);
}

static const Scalar blue(255, 0, 0);
static const Scalar red(0, 0, 255);

MatColor WmbVision::getSegmentation() const
{
  MatColor rgb;
  cvtColor(canny_, rgb, CV_GRAY2BGR);
  drawCircle(rgb, wheelL_, blue);
  drawCircle(rgb, Vec3f(wheelL_[0], wheelL_[1], 2), red);
  drawCircle(rgb, wheelR_, blue);
  drawCircle(rgb, Vec3f(wheelR_[0], wheelR_[1], 2), red);
  return rgb;
}

} // namespace wmb
