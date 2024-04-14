#include "collisions.h"

static float triangleSign(const Vector2& p1, const Vector2& p2, const Vector2& p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static bool pointInTriangle(const Triangle2D& triangle, const Vector2& center) {
  auto d1 = triangleSign(center, triangle.p1, triangle.p2);
  auto d2 = triangleSign(center, triangle.p2, triangle.p3);
  auto d3 = triangleSign(center, triangle.p3, triangle.p1);
  auto hasNeg = d1 < 0 || d2 < 0 || d3 < 0;
  auto hasPos = d1 > 0 || d2 > 0 || d3 > 0;
  return !(hasNeg && hasPos);
}

bool CheckCollisionLineCircle(const Vector2& p1, const Vector2& p2, const Vector2& center, float radiusSqr) {
  auto temp = p2 - p1;
  auto temp1 = center - p1;
  auto len = Vector2Length(temp);
  auto norm = Vector2Normalize(temp);
  auto dot = Vector2DotProduct(temp1, norm);
  Vector2 res{};
  if (dot <= 0) {
    res = p1;
  } else if (dot >= len) {
    res = p2;
  } else {
    res = Vector2Scale(norm, dot) + p1;
  }

  res = center - res;

  return Vector2LengthSqr(res) <= radiusSqr;
}

bool CheckCollisionTriangleCircle(const Triangle2D& triangle, const Vector2& center, float radius) {
  // 1. if any of triangle vertices is inside the circle => intersection
  auto radius2 = radius * radius;
  if (Vector2DistanceSqr(triangle.p1, center) < radius2 || Vector2DistanceSqr(triangle.p2, center) < radius2 ||
      Vector2DistanceSqr(triangle.p3, center) < radius2) {
    return true;
  }

  // 2. if circle center is inside the triangle => intersection
  if (pointInTriangle(triangle, center)) {
    return true;
  }

  // 3. If any of line intersects with the circle => intersection
  if (CheckCollisionLineCircle(triangle.p1, triangle.p2, center, radius2) ||
      CheckCollisionLineCircle(triangle.p2, triangle.p3, center, radius2) ||
      CheckCollisionLineCircle(triangle.p3, triangle.p1, center, radius2)) {
    return true;
  }

  return false;
}

bool CircleInCircle(const Vector2& p1, float radius1, const Vector2& p2, float radius2) {
  return Vector2Distance(p1, p2) < radius1 - radius2;
}
