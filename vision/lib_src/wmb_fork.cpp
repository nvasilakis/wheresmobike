
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

//static float distSq(float x0, float y0, float x1, float y1)
//{
//  const float dx = x0-x1;
//  const float dy = y0-y1;
//  return dx*dx + dy*dy;
//}

static bool isGoodLine(const Vec3f &wheel, const float maxDistSq,
    const Vec2f &line, const bool leftSide)
{
  const float rho = line[0];
  const float theta = line[1];


  if(leftSide) {
    if(theta > M_PI_2) {
      return false;
    }
  } else {
    if(theta < M_PI_2) {
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
  if(ret) {
    DEBUG(rho);
    DEBUG(theta);
    DEBUG(leftSide);

  }
  return ret;
}

static void findLinesFromCircle(const Lines &allLines, const Vec3f &wheel,
    Lines &goodLines, const float maxDistSq, const bool leftSide)
{
  goodLines.clear();

  DEBUG(wheel[0]);
  DEBUG(wheel[1]);
  DEBUG(wheel[2]);
  DEBUG(maxDistSq);

  for(const Vec2f &line : allLines) {
    // check original orientation
    if(isGoodLine(wheel, maxDistSq, line, leftSide)) {
      goodLines.push_back(line);
    }
  } // for each Line
} // void findLinesFromCircle(...)

bool WmbVision::findFork()
{
  // find fork
  HoughLines(canny_, allLines_, linesDistanceResolution(), linesAngleResolution(),
      linesAccumThresh(wheelRadius_));

  const float maxDistSq = maxLineDistSqFromCircle(wheelRadius_);

  findLinesFromCircle(allLines_, wheelL_, linesL_, maxDistSq, true);
  findLinesFromCircle(allLines_, wheelR_, linesR_, maxDistSq, false);

  if(linesL_.empty() && linesR_.empty()) return false;

  return true;
}

} // namespace wmb
