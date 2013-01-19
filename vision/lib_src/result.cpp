/*
 * result.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: nstiurca
 */

#include "wmb/result.h"

using namespace std;
using namespace cv;

namespace wmb
{

void Result::write(FileStorage &fs) const
{
  fs << "{";
    fs << "postid" << postid;
    fs << "terms" << terms;
    fs << "confidence" << confidence;
  fs << "}";
}

void writeAll(const Results &results, cv::FileStorage &fs)
{
//  fs << "results" << "[";
    for(const Result &result : results) {
      fs << "result" << result;
    }
//  fs << "]";
}

} // namespace wmb
