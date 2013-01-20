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

EStatus doIt(CvKNearest &knn, WmbVision &wmb,
             const std::vector<std::string> &ids)
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

  MatResult samples;

  for(unsigned i=0; i<n; ++i) {
    cin >> imageFileName;
    image = imread(imageFileName, CV_LOAD_IMAGE_COLOR);

    if(image.empty()) {
      return EStatus::READ_ERROR;
    }

    bool success = wmb.process(image);

    if (!success) continue;

    MatResult features = wmb.getFeatures();
    DEBUG(features.cols);
    samples.push_back(features);
    DEBUG(samples.rows);
  }

  DEBUG(samples.rows);
  MatResult neighborResponses;
  MatResult dist;
  knn.find_nearest(samples, K, nullptr, nullptr, &neighborResponses, &dist);
  CV_DbgAssert(K == neighborResponses.cols);
  CV_DbgAssert(K == dist.cols);

  std::unordered_map<int, float> votes;
  const int lim = neighborResponses.cols * neighborResponses.rows;
  for (int i = 0; i < lim; i++) {
    int id = neighborResponses(i);
    ++votes[id];
  }

  int best_id = -1;
  float most_votes = 0;

  for (const auto &v : votes) {
    if (v.second > most_votes) {
      most_votes = v.second;
      best_id = v.first;
    }
  }
  DEBUG(best_id);
  DEBUG(most_votes);

  Results results;

  results.emplace_back(ids[best_id], "", most_votes);

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

  MatResult trainData(r.size(), 2);
  CV_DbgAssert(trainData.cols == 2);
  std::vector<std::string> ids(r.size());
  MatResult responses(r.size(), 1);

  for (unsigned i = 0; i < r.size(); i++) {
    BikeFeatures bf;
    r[i] >> bf;
//    DEBUG(bf.postid);
    trainData.row(i) = bf.features;
    ids[i] = bf.postid;
    responses(i) = i;
  }

  CvKNearest knn;
  CvMat trainData2 = trainData;
  CvMat responses2 = responses;
  knn.train(&trainData2, &responses2, nullptr, false, K, false);

  cout << "HELO" << endl;

  for(ever) {
    try {
      EStatus status = doIt(knn, wmb, ids);
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
