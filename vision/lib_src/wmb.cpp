
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
//  if(!findFork()) return false;
  findFork();

#ifndef NDEBUG
  imshow("img", img);
  imshow("smallBlurred", smallGray_);
  imshow("canny", canny_);
  displayCircles(smallGray_, allCircles_, "all");
  displayCircles(smallGray_, Circles({wheelL_, wheelR_}), "wheels");
  displayLines(canny_, allLines_, "all");
  displayLines(canny_, linesL_, "left");
  displayLines(canny_, linesR_, "right");
  waitKey(0);
#endif

  return true;
}

cv::Mat_<double> WmbVision::getFeatures() const
{

  cv::Mat_<double> res(2, 1);

  double x1 = wheelL_[0];
  double y1 = wheelL_[1];
  double x2 = wheelR_[0];
  double y2 = wheelR_[1];

  double dist = sqrt((x1 - x2) * (x1 - x2) +
                     (y1 - y2) * (y1 - y2));

  res(0) = wheelRadius_ / dist;
  res(1) = 42.42;

  return res;

}

} // namespace wmb
