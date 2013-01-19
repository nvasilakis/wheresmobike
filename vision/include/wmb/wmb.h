/*
 * wmb.h
 *
 *  Created on: Jan 18, 2013
 *      Author: nstiurca
 */
#pragma once
#ifndef WMB_H_
#define WMB_H_

#include <string>
#include <vector>
#include <unordered_map>

#include <opencv2/core/core.hpp>

namespace wmb
{

typedef cv::Mat_<cv::Vec3b> MatColor;
typedef cv::Mat_<uint8_t> MatGray;
typedef std::vector<cv::Vec3f> Circles;
typedef std::vector<cv::Vec2f> Lines;
typedef std::vector<std::string> Strings;
typedef std::unordered_map<std::string, MatColor> Images;

struct Bike
{
  std::string url;
  std::string color;
  std::string type;
  Strings make_or_model;
  Images images;

  void read(const cv::FileNode &n);
  void write(cv::FileStorage &fs) const;
};

typedef std::vector<Bike> Bikes;
Bikes loadBikes(const char * const filename);

class WmbVision
{
  MatColor small_;
  MatGray smallGray_;
  MatGray canny_;
  Circles circles_, circlesR_;
  Lines lines_;

  const double cannyThreshHigh_;
  const double cannyThreshLow_;

public:
  WmbVision(const double cannyThresh0, const double cannyThresh1)
    : cannyThreshHigh_(std::max(cannyThresh0, cannyThresh1))
    , cannyThreshLow_(std::min(cannyThresh0, cannyThresh1))
  {}

  bool process(const MatColor &img);
  uint8_t getDominantHue() const;

}; // class WmbVision

} // namespace wmb

#endif /* WMB_H_ */
