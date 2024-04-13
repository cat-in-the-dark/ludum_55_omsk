#include "renderer.h"

#include <rlgl.h>

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

void DrawTriangleLinesThick(const Triangle2D& triangle, float thickness, Color color) {
  DrawLineThick(triangle.p1, triangle.p2, thickness, color);
  DrawLineThick(triangle.p2, triangle.p3, thickness, color);
  DrawLineThick(triangle.p3, triangle.p1, thickness, color);
}

void DrawLines(std::vector<Vector2>& model, Color color) {
  rlBegin(RL_LINES);
  rlColor4ub(color.r, color.g, color.b, color.a);
  for (size_t i = 0; i < model.size() - 1; i++) {
    rlVertex2f(model[i].x, model[i].y);
    rlVertex2f(model[i + 1].x, model[i + 1].y);
  }

  auto& last = model[model.size() - 1];
  rlVertex2f(last.x, last.y);
  rlVertex2f(model[0].x, model[0].y);
  rlEnd();
}

void DrawLines(std::vector<Vector2>& model, float thickness, Color color) {
  for (size_t i = 0; i < model.size() - 1; i++) {
    DrawLineThick(model[i], model[i + 1], thickness, color);
  }
  DrawLineThick(model[model.size() - 1], model[0], thickness, color);
}
