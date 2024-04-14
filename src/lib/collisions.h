#pragma once

#include "types.h"

bool CheckCollisionLineCircle(const Vector2& p1, const Vector2& p2, const Vector2& center, float radiusSqr);

bool CheckCollisionTriangleCircle(const Triangle2D& triangle, const Vector2& center, float radius);

/// @brief Check if one (smaller) circle is fully inside another (larger) circle
/// @param p1 Larger circle center
/// @param radius1 Larger circle radius
/// @param p2 Smaller circle center
/// @param radius2 Smaller circle radius
/// @return True if circle {p2, radius2} lays inside circle {p1, radius1}
bool CircleInCircle(const Vector2& p1, float radius1, const Vector2& p2, float radius2);
