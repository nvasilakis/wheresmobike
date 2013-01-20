
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

static void makeItSmall(const MatColor &img, MatColor &small, double &scale)
{
  const int MAX_ROWS = 160;
  const int NOMINAL_COLS = 160;

  scale = double(NOMINAL_COLS)/img.cols;
  const int rows = roundToNearestInt(img.rows * scale);
  if(rows <= MAX_ROWS) {
    resize(img, small, Size(NOMINAL_COLS, rows));
  } else {
    scale = double(MAX_ROWS)/img.rows;
    const int cols = roundToNearestInt(img.cols * scale);
    CV_DbgAssert(cols <= NOMINAL_COLS);
    resize(img, small, Size(cols, MAX_ROWS));
  }

  DEBUG(small.rows);
  DEBUG(small.cols);
}

void WmbVision::proprocessImage(const MatColor& img)
{
  // preprocess image
  makeItSmall(img, small_, scale_);
  cvtColor(small_, smallGray_, CV_BGR2GRAY);
  GaussianBlur(smallGray_, smallGray_, Size(5, 5), 0.0, 0.0);
  Canny(smallGray_, canny_, cannyThreshHigh_, cannyThreshLow_);
}

} // namespace wmb
