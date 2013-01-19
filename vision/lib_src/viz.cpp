/*
 * viz.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */

#include <opencv2/highgui/highgui.hpp>

#include "wmb/logging.h"
#include "wmb/viz.h"

using namespace std;
using namespace cv;

namespace wmb
{

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

} // namespace wmb
