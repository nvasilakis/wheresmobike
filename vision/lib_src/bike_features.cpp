
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
    n["id"] >> id;
    n["features"] >> features;
  }

  void BikeFeatures::write(cv::FileStorage &fs) const {
    fs << "{" << "id" << id << "features" << features << "}";
  }

void write(cv::FileStorage& fs, const std::string&, const BikeFeatures& bf)
{
  bf.write(fs);
}

} // namespace wmb
