#pragma once

#include <raylib.h>

#if (defined(__GNUC__) || defined(__clang__))
// Synonim for #pragma clang diagnostic
// see https://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-via-pragmas
#pragma GCC diagnostic push
// suppress "missing field 'y' initializer" in the code like this:
// Vector2 result = { 0 };
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#include <raymath.h>
#if (defined (__GNUC__) || defined(__clang__))
#pragma GCC diagnostic pop
#endif

struct Triangle2D {
  Vector2 p1;
  Vector2 p2;
  Vector2 p3;
};

struct Circle {
  Vector2 center;
  float radius;
};

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
  return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
  return {v1.x + v2.x, v1.y + v2.y};
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
  return {v1.x - v2.x, v1.y - v2.y};
}

inline Vector2 operator*(const Vector2& v, float scale) {
  return {v.x * scale, v.y * scale};
}
