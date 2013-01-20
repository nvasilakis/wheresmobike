/*
 * wmb_ctor.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */


#include "wmb/wmb.h"

#ifndef NDEBUG
#include "wmb/viz.h"
#endif

using namespace std;
using namespace cv;

namespace wmb
{

WmbVision::WmbVision(double cannyThresh0 , double cannyThresh1)
  : wheelRadius_(-1.0)
  , cannyThreshHigh_(std::max(cannyThresh0, cannyThresh1))
  , cannyThreshLow_(std::min(cannyThresh0, cannyThresh1))
{}

WmbVision::~WmbVision()
{
#ifndef NDEBUG
  vizCleanup();
#endif
}

} // namespace wmb
