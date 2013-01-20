/*
 * wmb_params.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */

#include <cmath>

#include "wmb/wmb_params.h"

namespace wmb
{

static const double TAU = 6.28318530718;

// wheel extraction
double houghCircleShrinkFactor()          { return 1.0; }
double minWheelDist(const int cols)       { return 0*cols + 1.0; } // { return cols * 0.166; }
double circleAccumThresh(const int cols)  { return cols * 0.166; }
int minWheelRadius(const int cols)        { return round(cols * 0.125); }
int maxWheelRadius(const int cols)        { return round(cols * 0.333); }

// fork extraction
double linesDistanceResolution()      { return 1.0; }
double linesAngleResolution()         { return 1.0*TAU/360.0; }
int linesAccumThresh(const double radius)  { return round(radius * 0.25); }
double minLineLength(const double radius)  { return radius * 0.75; }
double maxLineGap(const double radius)     { return radius * 0.35; }
double maxLineDistSqFromCircle(const double radius) { return (radius * 0.25) * (radius * 0.25); }
double minForkAngle()   { return 15.0 * TAU / 360.0; }
double optimalForkAngle() { return 60.0 * TAU / 360.0; }
double maxForkAngle()   { return 105.0 * TAU / 360.0; }

} // namespace wmb
