/*
 * magic.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

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

EStatus doIt(Ptr<DescriptorMatcher> &matcher, WmbVision &wmb,
             const vector<string> &ids)
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

  MatFeatures queryDescriptors;

  for(unsigned i=0; i<n; ++i) {
    cin >> imageFileName;
    image = imread(imageFileName, CV_LOAD_IMAGE_COLOR);

    if(image.empty()) {
      return EStatus::READ_ERROR;
    }

    bool success = wmb.process(image);

    if (!success) continue;

    MatFeatures features = wmb.getFeatures();
    DEBUG(features(0));
    queryDescriptors.push_back(features);
  }

  DEBUG(queryDescriptors.rows);
  vector<vector<DMatch>> matches;
  matcher->knnMatch(queryDescriptors, matches, K);


  std::unordered_map<int, float> votes;
  for (const vector<DMatch> &dmatches : matches) {
    for(const DMatch &dmatch : dmatches) {
      DEBUG(dmatch.trainIdx);
      DEBUG(dmatch.imgIdx);
      DEBUG(dmatch.distance);
      votes[dmatch.imgIdx] += 1.0f/dmatch.distance;
    }
  }

//  int best_id = -1;
//  float most_votes = 0;
//
//  for (const auto &v : votes) {
//    if (v.second > most_votes) {
//      most_votes = v.second;
//      best_id = v.first;
//    }
//  }
//  DEBUG(best_id);
//  DEBUG(most_votes);

  Results results;
  results.reserve(votes.size());

  for(const auto &v : votes) {
    results.emplace_back(ids[v.first], "", v.second);
  }

  writeAll(results, fs);
  fs.release();


  string outputSegmentationFileName;
  cin >> outputSegmentationFileName;
  Mat segmentation = wmb.getSegmentation();
  if(!imwrite(outputSegmentationFileName, segmentation)) {
    return EStatus::WRITE_ERROR;
  }

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

  unordered_map<string, MatFeatures> posts;
  posts.reserve(r.size());
  for (unsigned i = 0; i < r.size(); i++) {
    BikeFeatures bf;
    r[i] >> bf;
    posts[bf.postid].push_back(bf.features);
  }

  DEBUG(posts.size());

  vector<string> ids;               ids.reserve(posts.size());
  vector<Mat> descriptors;  descriptors.reserve(posts.size());
  for(const auto & post : posts) {
    ids.emplace_back(post.first);
    descriptors.emplace_back(post.second.clone());
    DEBUG(post.second.rows);
    CV_DbgAssert(descriptors.back().type() == CV_32FC1);
  }
  assert(posts.size() == ids.size());
  assert(posts.size() == descriptors.size());

  DEBUG_STR("Creating Flann based matcher");
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
  CV_Assert(matcher);
  DEBUG_STR("Adding descriptors");
  matcher->add(descriptors);
  DEBUG_STR("training");
  matcher->train();

  cout << "HELO" << endl;

  for(ever) {
    try {
      EStatus status = doIt(matcher, wmb, ids);
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
