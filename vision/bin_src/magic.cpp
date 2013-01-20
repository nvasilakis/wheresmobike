/*
 * magic.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include "wmb/logging.h"
#include "wmb/result.h"
#include "wmb/bike_features.h"
#include "wmb/wmb.h"

using namespace std;
using namespace cv;
using namespace wmb;

#define ever ;;

const int K = 20;

enum class EStatus
{
  OK,
  BAD_INPUT,
  NO_RESULTS,
  READ_ERROR,
  WRITE_ERROR,
  END_OF_INPUT
};

EStatus doIt(CvKNearest &knn, WmbVision &wmb)
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

  cv::Mat_<float> samples;
  cv::Mat_<float> knnResults;

  for(int i=0; i<n; ++i) {
    cin >> imageFileName;
    image = imread(imageFileName, CV_LOAD_IMAGE_COLOR);

    if(image.empty()) {
      return EStatus::READ_ERROR;
    }

    bool success = wmb.process(image);

    if (!success) continue;

    cv::Mat_<float> features = wmb.getFeatures();
    samples.push_back(features);
  }

  knn.find_nearest(samples, K, &knnResults);

  results.emplace_back("3502549351", outputFileName, 666);

  writeAll(results, fs);
  fs.release();

  return results.empty() ? EStatus::NO_RESULTS : EStatus::OK;
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    FATAL_STR("Wrong number of arguments (should be 2).");
    exit(-1);
  }

  WmbVision wmb(150.0, 75.0);

  FileStorage fs(argv[1], FileStorage::READ);

  FileNode r = fs["bike_features"];
  CV_Assert(r.isSeq());

  cv::Mat_<float> trainData(r.size(), 2);
  CV_DbgAssert(trainData.cols == 2);
  std::vector<std::string> ids(r.size());
  cv::Mat_<float> responses(r.size(), 1);

  for (int i = 0; i < r.size(); i++) {
    BikeFeatures bf;
    r[i] >> bf;
    trainData.row(i) = bf.features;
    ids[i] = bf.id;
    responses(i) = i;
  }

  CvKNearest knn;
  CvMat trainData2 = trainData;
  CvMat responses2 = responses;
  knn.train(&trainData2, &responses2, nullptr, false, K, false);

  for(ever) {
    try {
      EStatus status = doIt(knn, wmb);
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
