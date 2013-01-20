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

  FileStorage bikesFs(filename, FileStorage::READ);
  if(!bikesFs.isOpened()) {
    FATAL_STR("could not open " << filename << " for reading");
    exit(-3);
  }
  const FileNode &bikesNode = bikesFs["bikes"];
  CV_Assert(bikesNode.isSeq());

  WmbVision wmb(150.0, 75.0);

  FileStorage fs(string("features_")+filename, FileStorage::WRITE);
  fs << "bike_features" << "[";

  Bike bike;
  for(int i=0; i<bikesNode.size(); ++i) {
    bike.read(bikesNode[i]);
    for(auto img : bike.images) {
      ++i;
      bool success = wmb.process(img.second);
      if(success) {
        BikeFeatures bf;
        bf.postid = bike.url;
        bf.imagename = img.first;
        bf.features = wmb.getFeatures();
        INFO(bf.features(0));
        INFO(bf.features(1));
        fs << bf;
        INFO(i);
      } else {
        WARN_STR("Could not process bike image " << img.first);
      }
    } // for each Image
  } // for each Bike

  fs << "]";
  fs.release();

  return 0;
}
