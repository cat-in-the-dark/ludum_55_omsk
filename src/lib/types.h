#pragma once

#include <raylib.h>

// Synonim for #pragma clang diagnostic
// see https://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-via-pragmas
#pragma GCC diagnostic push
// suppress "missing field 'y' initializer" in the code like this:
// Vector2 result = { 0 };
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <raymath.h>
#pragma GCC diagnostic pop

struct Triangle2D {
  Vector2 p1;
  Vector2 p2;
  Vector2 p3;
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
