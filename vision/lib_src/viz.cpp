/*
 * viz.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */

#include <opencv2/highgui/highgui.hpp>

#include "wmb/logging.h"
#include "wmb/viz.h"
#include "wmb/round.h"

using namespace std;
using namespace cv;

namespace wmb
{

static const Scalar blue(255, 0, 0);
static const Scalar green(0, 255, 0);
static const Scalar red(0, 0, 255);
static const Scalar fuscia(255, 0, 255);
//static const Scalar yellow(0, 200, 200);

void displayBike(const Bike &bike)
{
  INFO(bike.url);
  INFO(bike.color);
  INFO(bike.type);

  for(const string &make_or_model : bike.make_or_model) {
    INFO(make_or_model);
  }

  for(const auto & img : bike.images) {
    imshow(img.first, img.second);
  }

  waitKey(0);

  destroyAllWindows();
  waitKey(1);
}

static MatColor makeRgb(const Mat &img)
{
//  assert(img.channels() == 1);
  if(img.channels() == 1) {
    MatGray layers[3] = {img * 0.5};
    layers[1] = layers[0].clone();
    layers[2] = layers[0].clone();
    MatColor rgb;
    merge(layers, 3, rgb);
    return rgb;
  } else if(img.channels() == 3) {
    return (MatColor)img.clone();
  } else {
    assert(false);
    return MatColor();
  }
}

static void drawCircle(MatColor &rgb, const Vec3f &ctrSz, const Scalar &color)
{
  Point c(roundToNearestInt(ctrSz[0]*16.0f), roundToNearestInt(ctrSz[1]*16.0f));
  int r = roundToNearestInt(ctrSz[2]*16.0f);
  circle(rgb, c, r, color, 1, CV_AA, 4);
}

void displayCircles(const MatGray &img, const Circles &circles, const string &name)
{
  MatColor rgb = makeRgb(img);
  for(const Vec3f &circle : circles) {
    drawCircle(rgb, circle, blue);
    drawCircle(rgb, Vec3f(circle[0], circle[1], 2), red);
  }
  imshow(name+" circles", rgb);
}

static void drawSegment(MatColor &rgb, float x0, float y0, float x1, float y1, const Scalar &color)
{
  Point pt0(roundToNearestInt(x0*16.0f), roundToNearestInt(y0*16.0f));
  Point pt1(roundToNearestInt(x1*16.0f), roundToNearestInt(y1*16.0f));
  line(rgb, pt0, pt1, color, 1, CV_AA, 4);
}

//static void drawSegment(MatColor &rgb, const Vec4i &p, const Scalar &color)
//{
//  line(rgb, Point(p[0], p[1]), Point(p[2], p[3]), color, 1, CV_AA, 0);
//  drawCircle(rgb, Vec3f(p[0], p[1], 1), fuscia);
//  drawCircle(rgb, Vec3f(p[2], p[3], 1), fuscia);
//}

void displayLines(const MatGray &img, const Lines &lines, const string &name)
{
  MatColor rgb = makeRgb(img);
  for(const Vec2f &line : lines) {
    const float rho = line[0];
    const float theta = line[1];
    const float a = cosf(theta),
                b = sinf(theta);
    const float x0 = a*rho,
                y0 = b*rho;
    Point pt1(cvRound(x0 + 1000*(-b)),
              cvRound(y0 + 1000*(a)));
    Point pt2(cvRound(x0 - 1000*(-b)),
              cvRound(y0 - 1000*(a)));
    cv::line(rgb, pt1, pt2, blue, 1, CV_AA, 0);
  }
  imshow(name+" lines", rgb);
}

void displayLineSegments(const MatGray &img, const vector<Vec4f> &segments, const string &name)
{
  MatColor rgb = makeRgb(img);
  for(const Vec4f &seg : segments) {
    drawSegment(rgb, seg[0], seg[1], seg[2], seg[3], fuscia);
  }
  imshow(name+" segments", rgb);
}

void vizCleanup()
{
  destroyAllWindows();
}

} // namespace wmb
