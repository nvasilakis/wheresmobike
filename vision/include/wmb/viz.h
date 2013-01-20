/*
 * viz.h
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */
#pragma once
#ifndef VIZ_H_
#define VIZ_H_

#include "wmb.h"

namespace wmb
{

void displayBike(const Bike &bike);
void displayCircles(const MatGray &img, const Circles &circles, const std::string &name = "");
void displayLines(const MatGray &img, const Lines &lines, const std::string &name="");

void vizCleanup();

} // namespace wmb

#endif /* VIZ_H_ */
