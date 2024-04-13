#include "renderer.h"

#include "lib/types.h"

void DrawLineThick(const Vector2& start, const Vector2& end, float thickness, Color color) {
  auto dif = end - start;
  auto rot = atan2(dif.y, dif.x);
  auto offset = thickness / 2;
  auto p1 = start + Vector2Rotate(Vector2{0, offset}, rot);
  auto p2 = start + Vector2Rotate(Vector2{0, -offset}, rot);
  auto p3 = end + Vector2Rotate(Vector2{0, offset}, rot);
  auto p4 = end + Vector2Rotate(Vector2{0, -offset}, rot);

  DrawTriangle(p1, p3, p2, color);
  DrawTriangle(p2, p3, p4, color);
}

void DrawLineThick(float x1, float y1, float x2, float y2, float thickness, Color color) {
  return DrawLineThick({x1, y1}, {x2, y2}, thickness, color);
}
