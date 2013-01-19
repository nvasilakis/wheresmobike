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

  FileStorage fs(filename, FileStorage::READ);
  if(!fs.isOpened()) {
    FATAL_STR("Could not open " << datafolder << "/" << filename << " for reading");
    exit(-3);
  }
  const FileNode &bikesNode = fs["bikes"];
  DEBUG(bikesNode.empty());
  DEBUG(bikesNode.isNone());
  DEBUG(bikesNode.isSeq());
  DEBUG(bikesNode.isMap());
  DEBUG(bikesNode.isInt());
  DEBUG(bikesNode.isReal());
  DEBUG(bikesNode.isString());
  DEBUG(bikesNode.isNamed());
  CV_Assert(bikesNode.type() == FileNode::SEQ);

  DEBUG(bikesNode.size());
  vector<Bike> bikes(bikesNode.size());
  for(int i=0; i<bikesNode.size(); ++i) {
    INFO(i);
    const auto &bn = bikesNode[i];
    auto &b = bikes[i];
    b.read(bn);

    displayBike(bikes[i]);
  }

  INFO_STR("Closing down");

  fs.release();
  bikes.clear();

  INFO_STR("done");

  return 0;
}
