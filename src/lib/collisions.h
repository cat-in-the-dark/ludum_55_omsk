#pragma once

#include "types.h"

bool CheckCollisionLineCircle(const Vector2& p1, const Vector2& p2, const Vector2& center, float radiusSqr);

bool CheckCollisionTriangleCircle(const Triangle2D& triangle, const Vector2& center, float radius);
