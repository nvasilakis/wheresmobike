/*
 * trainer.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */

#include <unistd.h> // chdir()

#include "wmb/wmb.h"
#include "wmb/logging.h"
#include "wmb/viz.h"
#include "wmb/bike_features.h"

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

  WmbVision wmb(150.0, 60.0);

  FileStorage fs("features.yaml", FileStorage::WRITE);

  for(Bike &bike : bikes) {
    for(auto img : bike.images) {
      bool success = wmb.process(img.second);
      if(success) {
        BikeFeatures bf;
        bf.id = bike.url;
        bf.features = wmb.getFeatures();
        INFO(bf.features(0));
        INFO(bf.features(1));
        fs << "bike_features" << bf;
      } else {
        WARN_STR("Could not process bike image " << img.first);
      }
    } // for each Image
  } // for each Bike

  return 0;
}
