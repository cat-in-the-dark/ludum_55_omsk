#pragma once

#include <cmath>

#include "types.h"

inline float easeOutCubic(float x) {
  return 1 - pow(1 - x, 3);
}

inline float easeInCubic(float x) {
  return x * x * x;
}

inline Vector2 Scale2D(Vector2 v, const Vector2& scale) {
  return {v.x * scale.x, v.y * scale.y};
}

inline Vector2 Lerp2D(const Vector2& v1, const Vector2& v2, float t) {
  return Vector2{Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t)};
}

inline Triangle2D Tri60() {
  return {{0, -1}, {-float(std::sqrt(3)) / 2.0f, 0.5}, {float(std::sqrt(3)) / 2.0f, 0.5}};
}

inline float ReLU(float x, float p) {
  if (x < p) {
    return 0;
  }
  return x;
}
