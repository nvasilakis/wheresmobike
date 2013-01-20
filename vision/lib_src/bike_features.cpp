
#include <algorithm>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "wmb/wmb.h"
#include "wmb/logging.h"
#include "wmb/round.h"
#include "wmb/wmb_params.h"
#include "wmb/bike_features.h"

#ifndef NDEBUG
#include "wmb/viz.h"
#endif

using namespace std;
using namespace cv;

namespace wmb
{

  void BikeFeatures::read(const cv::FileNode &n)
  {
    n["postid"] >> postid;
    n["imagename"] >> imagename;
    n["features"] >> features;
  }

  void BikeFeatures::write(cv::FileStorage &fs) const {
    fs << "{"
        << "postid" << postid
        << "imagename" << imagename
        << "features" << features
       << "}";
  }


  void read(const cv::FileNode &fn, BikeFeatures &bf, const BikeFeatures & default_value) {
    if (fn.empty()) {
      bf = default_value;
    } else {
      bf.read(fn);
    }
  }

  void write(cv::FileStorage& fs, const std::string&, const BikeFeatures& bf) {
    bf.write(fs);
  }

} // namespace wmb
