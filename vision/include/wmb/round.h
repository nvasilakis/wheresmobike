#pragma once

#include <cmath>

namespace wmb
{

constexpr inline int roundToNearestInt(float f)
{
  return floor(f+0.5f);
}

} // namespace wmb
