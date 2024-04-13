#pragma once

#include <raylib.h>

#include <vector>

#include "lib/types.h"

void DrawLineThick(const Vector2& p1, const Vector2& p2, float thickness, Color color);

void DrawLineThick(float x1, float y1, float x2, float y2, float thickness, Color color);

void DrawTriangleLinesThick(const Triangle2D& triangle, float thickness, Color color);

void DrawLines(std::vector<Vector2>& model, Color color);

void DrawLines(std::vector<Vector2>& model, float thickness, Color color);
