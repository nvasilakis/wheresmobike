/*
 * bike_persistance.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */

#include <opencv2/highgui/highgui.hpp>

#include "wmb/wmb.h"
#include "wmb/logging.h"

using namespace std;
using namespace cv;

namespace wmb
{

static void readStrings(const FileNode &n, Strings &strings)
{
  strings.resize(n.size());
  CV_Assert(n.type() == FileNode::SEQ);
  for(int i=0; i<n.size(); ++i) {
    CV_Assert(n[i].type() == FileNode::STR);
    n[i] >> strings[i];
  }
}

static void loadImages(const Strings &names, Images &images)
{
  for(const string &name : names) {
    images[name] = imread(name, CV_LOAD_IMAGE_COLOR);
    CV_Assert(!images[name].empty());
  }
}

/**
 * read a Bike from a FileNode
 */
void Bike::read(const FileNode &n)
{
  CV_Assert(n.type() == FileNode::MAP);

  n["url"] >> url;
  n["color"] >> color;
  n["type"] >> type;

  readStrings(n["make_or_model"], make_or_model);

  Strings imageNames;
  readStrings(n["images"], imageNames);
  loadImages(imageNames, images);
}

/**
 * Write a Bike to FileStorage
 */
void Bike::write(FileStorage &fs) const
{
  fs << "{";
    fs << "color" << color;
    fs << "type" << type;
    fs << "url" << url;

    fs << "make_or_model" << "[:";
      for(const string &m : make_or_model) {
        fs << m;
      }
    fs << "]";

    fs << "images" << "[";
      for(const auto &im : images) {
        fs << im.first;
      }
    fs << "]";
  fs << "}";
}

Bikes loadBikes(const char * const filename)
{
  FileStorage fs(filename, FileStorage::READ);
  if(!fs.isOpened()) {
    FATAL_STR("Could not open " << filename << " for reading");
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
  Bikes bikes(bikesNode.size());
  for(int i=0; i<bikesNode.size(); ++i) {
    INFO(i);
    const auto &bn = bikesNode[i];
    Bike &b = bikes[i];
    b.read(bn);
  }

  fs.release();

  return bikes;
}

} // namespace wmb

