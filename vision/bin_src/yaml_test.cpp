/*
 * yaml_test.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */

#include <unistd.h> // chdir()

#include "wmb/logging.h"
#include "wmb/wmb.h"
#include "wmb/viz.h"

using namespace std;
using namespace cv;
using namespace wmb;

int main(int argc, char **argv)
{
  if(3 != argc) {
    FATAL_STR("usage: " << argv[0] << " <data-folder> <yaml-file>");
    exit(-1);
  }

  char const * const datafolder = argv[1];
  char const * const filename = argv[2];

  if(chdir(datafolder)) {
    FATAL_STR("Could not chdir " << datafolder);
    exit(-2);
  }

  Bikes bikes = loadBikes(filename);

  for(const Bike &bike : bikes) {
    displayBike(bike);
  }

  return 0;
}
