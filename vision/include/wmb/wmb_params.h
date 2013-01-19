/*
 * wmb_params.h
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */
#pragma once
#ifndef WMB_PARAMS_H_
#define WMB_PARAMS_H_

namespace wmb
{

// wheel extraction
extern double houghCircleShrinkFactor();
extern double minWheelDist(const int cols);
extern double circleAccumThresh(const int cols);
extern int minWheelRadius(const int cols);
extern int maxWheelRadius(const int cols);

// fork extraction
extern double linesDistanceResolution();
extern double linesAngleResolution();
extern int linesAccumThresh(const double radius);
extern double minLineLength(const double radius);
extern double maxLineGap(const double radius);

} // namespace wmb

#endif /* WMB_PARAMS_H_ */
