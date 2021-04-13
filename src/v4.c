#include "v4.h"

v4_t v4_create(float x, float y, float z, float w)
{
  v4_t result;
  result.x = x;
  result.y = y;
  result.z = z;
  result.w = w;
  return result;
}