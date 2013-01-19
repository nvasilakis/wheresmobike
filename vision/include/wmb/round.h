#pragma once

#include <cmath>

namespace wmb
{

constexpr inline int roundToNearestInt(float f)
{
  return floorf(f+0.5f);
}
constexpr inline int roundToNearestInt(double d)
{
  return floor(d+0.5);
}

} // namespace wmb
