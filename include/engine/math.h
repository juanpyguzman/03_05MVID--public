//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __ENGINE_DEFINES_H__
#define __ENGINE_DEFINES_H__ 1

#include "mathlib/vec2.h"

#include <algorithm>
#include <corecrt_math_defines.h>
#define NOMINMAX

template <typename T>
inline T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

inline MathLib::Vec2 rotate2D(const MathLib::Vec2& pivot, const MathLib::Vec2& point, const float angle) {
  const float s = sin(angle);
  const float c = cos(angle);

  return MathLib::Vec2(c * (point.x() - pivot.x()) - s * (point.y() - pivot.y()) + pivot.x(),
    s * (point.x() - pivot.x()) + c * (point.y() - pivot.y()) + pivot.y());
}

//random value between two numbers
inline float randomFloat(float a, float b) {
  const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return a + (random * (b - a));
}

//wrap an angle between [-PI, PI)z
inline float wrapAnglePI(double x) {
  x = fmod(x + M_PI, M_PI * 2.0f);
  if (x < 0) x += M_PI * 2.0f;
  return x - M_PI;
}

//wrap an angle between [-180, 180)
inline float wrapAngle180(float x) {
  x = fmod(x + 180.0f, 360.0f);
  if (x < 0) x += 360.0f;
  return x - 180.0f;
}

//wrap an angle between [0, 2 PI)
inline float wrapAngle2PI(float x) {
  x = fmod(x, M_PI * 2.0f);
  if (x < 0) x += M_PI * 2.0f;
  return x;
}

//wrap an angle between [0, 360)
inline float wrapAngle360(float x) {
  x = fmod(x, 360.0f);
  if (x < 0) x += 360.0f;
  return x;
}

//returns (-1, 0 , 1), the sign of the number
template <typename T> int sign(T val) {
  return (T(0) < val) - (val < T(0));
}

#endif