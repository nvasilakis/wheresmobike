
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

static float distSq(float x0, float y0, float x1, float y1)
{
  const float dx = x0-x1;
  const float dy = y0-y1;
  return dx*dx + dy*dy;
}

static bool isGoodLine(const Vec3f &wheel, const float maxDistSq,
    const Vec4i &line, const int dxMul)
{
  const float d2 = distSq(wheel[0], wheel[1], line[0], line[1]);
  if(d2 <= maxDistSq) {
    // not too far. check the angle
    const int dx = line[2] - line[0];
    const int dy = line[3] - line[1];
    DEBUG(line[0]);
    DEBUG(line[1]);
    DEBUG(d2);
    return dx * dxMul >=0 && dy <= 0;
  } // if(d2 <= maxDistSq)
  return false;
}

static void findLinesFromCircle(const Lines &allLines, const Vec3f &wheel,
    Lines &goodLines, const float maxDistSq, const int dxMul)
{
  goodLines.clear();

  DEBUG(wheel[0]);
  DEBUG(wheel[1]);
  DEBUG(wheel[2]);
  DEBUG(maxDistSq);

  for(const Vec4i &line : allLines) {
    // check original orientation
    if(isGoodLine(wheel, maxDistSq, line, dxMul)) {
      goodLines.push_back(line);
    } else {
      Vec4i l2(line[2], line[3], line[0], line[1]);
      if(isGoodLine(wheel, maxDistSq, l2, dxMul)) {
        goodLines.push_back(l2);
      }
    }
  } // for each Line
} // void findLinesFromCircle(...)

bool WmbVision::findFork()
{
  // find fork
  HoughLinesP(canny_, allLines_, linesDistanceResolution(), linesAngleResolution(),
      linesAccumThresh(wheelRadius_), minLineLength(wheelRadius_),
      maxLineGap(wheelRadius_));

  const float maxDistSq = maxLineDistSqFromCircle(wheelRadius_);

  findLinesFromCircle(allLines_, wheelL_, linesL_, maxDistSq, 1);
  findLinesFromCircle(allLines_, wheelR_, linesR_, maxDistSq, -1);

  if(linesL_.empty() && linesR_.empty()) return false;

  return true;
}

} // namespace wmb
