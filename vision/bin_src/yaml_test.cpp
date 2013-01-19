/*
 * yaml_test.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */

#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

#include "wmb/logging.h"
#include "wmb/wmb.h"

using namespace cv;
using namespace std;

vector<string> getStrings(const FileNode &n)
{
  CV_Assert(n.type() == FileNode::SEQ);
  vector<string> strings;
  for(int i=0; i<n.size(); ++i) {
    strings.push_back(n[i]);
  }
  return strings;
}

struct Bike
{
  string url;
  string color;
  string type;
  vector<string> make_or_model;
  vector<string> imageNames;
};

void readTest(char const * const fileName,
    char const * const dataFolder)
{
  FileStorage fs;
  fs.open(fileName , FileStorage::READ);
  if(!fs.isOpened())
  {
    FATAL_STR("Failed to open " << fileName);
    exit(-1);
  }

  FileNode bikesNode = fs["bikes"];
  FileNodeIterator bikesIt = bikesNode.begin(), bikesItEnd = bikesNode.end();
  for(; bikesIt != bikesItEnd; ++bikesIt) {
    FileNode bikeNode = *bikesIt;
    string url = bikeNode["url"];
    string color = bikeNode["color"];
    string type = bikeNode["type"];
    vector<string> make_or_model = getStrings(bikeNode["make_or_model"]);
    vector<string> imageNames = getStrings(bikeNode["images"]);

    DEBUG(url);
    DEBUG(color);
    DEBUG(type);
    for(const string &make : make_or_model) {
      DEBUG(make);
    }
    for(const string &imageName : imageNames) {
      DEBUG(imageName);
    }
  }
}

void writeTest(const char * const fileName)
{
  FileStorage fs(fileName, FileStorage::WRITE);

  struct Bike bike{
    "http://url",
    "red",
    "type",
    {"make", "model"},
    {"1.jpg", "2.jpg", "3.jpg"}
  };

  fs << "bikes"  << "[";
  for(int i=0; i<4; ++i) {
    fs << /*"bike" <<*/ "{";
    fs << "color" << bike.color;
    fs << "type" << bike.type;
    fs << "url" << bike.url;
    fs << "make_or_model" << "[:";
    for(const string &m : bike.make_or_model) {
      fs << m;
    }
    fs << "]" << "images" << "[";
    for(const string &i : bike.imageNames) {
      fs << i;
    }
    fs << "]" << "}";
  }
  fs << "]";
}

int main(int argc, char **argv)
{
  if(3 != argc) {
    FATAL_STR("second argumnt should be yaml");
    return -1;
  }

  char const * const filename = argv[1];
  char const * const datafolder = argv[2];

  writeTest(filename);


  return 0;
}
