/*
 * magic.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */
#include <iostream>

#include <opencv2/highgui/highgui.hpp>

#include "wmb/logging.h"
#include "wmb/result.h"

using namespace std;
using namespace cv;
using namespace wmb;

#define ever ;;

enum class EStatus
{
  OK,
  BAD_INPUT,
  NO_RESULTS,
  READ_ERROR,
  WRITE_ERROR,
  END_OF_INPUT
};

EStatus doIt()
{
  unsigned n;
  string outputFileName;
  string imageFileName;
  Mat image;

  cin >> n;
  if(!n) {
    return cin.eof() ? EStatus::END_OF_INPUT : EStatus::BAD_INPUT;
  }

  cin >> outputFileName;
  FileStorage fs(outputFileName, FileStorage::WRITE);
  if(!fs.isOpened()) {
    return EStatus::WRITE_ERROR;
  }

  Results results;

  for(int i=0; i<n; ++i) {
    cin >> imageFileName;
    image = imread(imageFileName, CV_LOAD_IMAGE_COLOR);
    if(image.empty()) {
      return EStatus::READ_ERROR;
    }
    if(i&1)
      results.emplace_back(imageFileName, outputFileName, ((double)(i))/n);
  }

  writeAll(results, fs);
  fs.release();

  return results.empty() ? EStatus::NO_RESULTS : EStatus::OK;
}

int main(int argc, char ** argv)
{
  for(ever) {
    try {
      EStatus status = doIt();
      switch(status) {
      case EStatus::OK:
        cout << "ACK" << endl;
        break;
      case EStatus::BAD_INPUT:
        cout << "BAD_INPUT" << endl;
        break;
      case EStatus::NO_RESULTS:
        cout << "NO_RESULTS" << endl;
        break;
      case EStatus::READ_ERROR:
        cout << "READ_ERROR" << endl;
        break;
      case EStatus::WRITE_ERROR:
        cout << "WRITE_ERROR" << endl;
        break;
      case EStatus::END_OF_INPUT:
        exit(0);
      default:
        FATAL((int)status);
        throw(status);
        break;
      }
    } catch (...) {
      cout << "NAK" << endl;
      ERROR_STR("Unknown exception");
    }
  }

  return 0;
}


