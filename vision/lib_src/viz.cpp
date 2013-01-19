/*
 * viz.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */

#include "wmb/logging.h"
#include "wmb/viz.h"

namespace wmb
{

void displayBike(const Bike &bike)
{
  WARN_STR("stub");
  INFO(bike.url);
  INFO(bike.color);
  INFO(bike.type);
}

} // namespace wmb
