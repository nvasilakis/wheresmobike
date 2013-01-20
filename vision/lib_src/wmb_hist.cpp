/*
 * wmb_hist.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: nstiurca
 */

#include <opencv2/imgproc/imgproc.hpp>
#ifndef NDEBUG
#include <opencv2/highgui/highgui.hpp>
#endif

#include "wmb/wmb.h"
#include "wmb/wmb_params.h"

using namespace cv;

namespace wmb
{

static Point rnd(const float x, const float y)
{
  return Point(cvRound(x*16.0f), cvRound(y*16.0f));
}

static void populateMask(Mat &mask, const Vec4f &seg)
{
  const float dx = seg[0] - seg[2];
  const float dy = seg[1] - seg[2];
  const float r = sqrt(dx*dx + dy*dy);
  line(mask, rnd(seg[0], seg[1]), rnd(seg[2], seg[3]), Scalar(255),
      forkWidth(r), 8, 4);
}

void WmbVision::buildColorHistograms(const MatColor &img)
{
  MatColor hsv;
  cvtColor(img, hsv, CV_BGR2HSV);

  // Quantize the hue to 30 levels
  // and the saturation to 32 levels
  int hbins = 30, sbins = 32;
  int histSize[] =
  { hbins, sbins };
  // hue varies from 0 to 179, see cvtColor
  float hranges[] =
  { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] =
  { 0, 256 };
  const float* ranges[] =
  { hranges, sranges };
  // we compute the histogram from the 0-th and 1-st channels
  int channels[] =
  { 0, 1 };

  Mat mask(img.size(), CV_8UC1, Scalar(0));
  CV_DbgAssert(scale_ < 1.0);
  populateMask(mask, forkL_ * (1.0 / scale_));
  populateMask(mask, forkR_ * (1.0 / scale_));

  calcHist(&hsv, 1, channels, mask,
      hist_, 2, histSize, ranges, true, // the histogram is uniform
      false);

#ifndef NDEBUG
  imshow("fork mask", mask);
#endif
}

} // namespace wmb
