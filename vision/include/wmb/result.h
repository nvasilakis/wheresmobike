/*
 * result.h
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */
#pragma once
#ifndef RESULT_H_
#define RESULT_H_

#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

namespace wmb
{

struct Result
{
  std::string postid;
  std::string terms;
  double confidence;

  Result() = default;
  Result(const std::string & postid, const std::string & terms, double confidence)
    : postid(postid)
    , terms(terms)
    , confidence(confidence)
  {}

  void write(cv::FileStorage &fs) const;
};

typedef std::vector<Result> Results;

void write(cv::FileStorage& fs, const std::string&, const Result& x)
{
  x.write(fs);
}

extern void writeAll(const Results &results, cv::FileStorage &fs);


} // namespace wmb

#endif /* RESULT_H_ */
