
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

static bool isGoodLine(const Vec3f &wheel, const float maxDistSq,
    const Vec2f &line, const bool leftSide)
{
  const float rho = line[0];
  const float theta = line[1];


  if(leftSide) {
    if(theta > optimalForkAngle() + maxForkAngleDeviation()) {
      return false;
    }
  } else {
    if(theta < M_PI - optimalForkAngle() - maxForkAngleDeviation()) {
      return false;
    }
  }

  const float dx = sin(-theta);
  const float dy = cos(-theta);
  const float b = -rho / dx;

  const float A = dy;
  const float B = -dx;
  const float C = dx*b;

  const float num = A*wheel[0] + B*wheel[1] + C;
  const float denSq = A*A + B*B;
  const float distSq = num*num/denSq;

  bool ret = distSq <= maxDistSq;
//  if(ret) {
//    DEBUG(rho);
//    DEBUG(theta);
//    DEBUG(leftSide);
//
//  }
  return ret;
}

static void findLinesFromCircle(const Lines &allLines, const Vec3f &wheel,
    Lines &goodLines, const float maxDistSq, const bool leftSide)
{
  goodLines.clear();

//  DEBUG(wheel[0]);
//  DEBUG(wheel[1]);
//  DEBUG(wheel[2]);
//  DEBUG(maxDistSq);

  for(const Vec2f &line : allLines) {
    // check original orientation
    if(isGoodLine(wheel, maxDistSq, line, leftSide)) {
      goodLines.push_back(line);
    }
  } // for each Line
} // void findLinesFromCircle(...)

typedef bool (*line_sort_fn)(const Vec2f &, const Vec2f &);

template <int N>
static bool sortBy(const Vec2f &a, const Vec2f &b)
  { return a[N] < b[N]; }

static constexpr bool isOdd(const int i) { return i&1; }
static constexpr bool isEven(const int i) { return !isOdd(i); }

template <int N>
static float findMedianLine(Lines & lines)
{
  DEBUG(lines.size());
  const auto b = lines.begin();
  const auto m = b+lines.size()/2;
  const auto e = lines.end();

  nth_element(b, m, e, sortBy<N>);
  float median = m[0][N];

  if(isEven(lines.size())) {
    nth_element(b, m-1, m, sortBy<N>);
    median += m[-1][N];
    median *= 0.5;
  }

  return median;
}

static void findBestLine(Lines &lines, const Vec3f & wheel,
    Vec4f &fork)
{
  if(lines.empty()) {
    fork = Vec4f();
    return;
  }

  const float medianTheta = findMedianLine<1>(lines);
  fork[0] = wheel[0];
  fork[1] = wheel[1];
  const float dx = sin(-medianTheta);
  const float dy = cos(-medianTheta);
  if(dy > 0.0) {
    fork[2] = wheel[0] - dx*wheel[2]*2.0;
    fork[3] = wheel[1] - dy*wheel[2]*2.0;
  } else {
    fork[2] = wheel[0] + dx*wheel[2]*2.0;
    fork[3] = wheel[1] + dy*wheel[2]*2.0;
  }
}

bool WmbVision::findForks()
{
  // find forks
  HoughLines(canny_, allLines_, linesDistanceResolution(), linesAngleResolution(),
      linesAccumThresh(wheelRadius_));

  const float maxDistSq = maxLineDistSqFromCircle(wheelRadius_);

  findLinesFromCircle(allLines_, wheelL_, linesL_, maxDistSq, true);
  findLinesFromCircle(allLines_, wheelR_, linesR_, maxDistSq, false);
  if(linesL_.empty() && linesR_.empty()) return false;

  findBestLine(linesL_, wheelL_, forkL_);
  findBestLine(linesR_, wheelR_, forkR_);

  return true;
}

} // namespace wmb
