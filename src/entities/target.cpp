#include "target.h"

#include <raylib.h>

#include "const.h"
#include "lib/math.h"
#include "lib/renderer.h"
#include "lib/types.h"

constexpr auto kTargetColor = LIME;
constexpr auto kRotationSpeed = PI / 4;

void Target::Update(float dt) {
  angle_ += kRotationSpeed * dt;
}

void Target::Draw() {
  auto radius = Vector2Rotate({kTargetSize / 2, 0.0f}, angle_);
  auto scaleY = Vector2{1.0f, 0.5f};
  auto topCenter = pos + Vector2{kTargetSize / 2, kTargetSize / 2};
  auto p1_top = topCenter + Scale2D(radius, scaleY);
  auto p2_top = topCenter + Scale2D(Vector2Rotate(radius, PI / 2), scaleY);
  auto p3_top = topCenter + Scale2D(Vector2Rotate(radius, PI), scaleY);
  auto p4_top = topCenter + Scale2D(Vector2Rotate(radius, 3 * PI / 2), scaleY);

  auto delta = Vector2{0.0f, kTargetSize * 2 / 3};

  auto p1_bottom = p1_top + delta;
  auto p2_bottom = p2_top + delta;
  auto p3_bottom = p3_top + delta;
  auto p4_bottom = p4_top + delta;

  DrawLineThick(p1_top, p2_top, kLineThickness, kTargetColor);
  DrawLineThick(p2_top, p3_top, kLineThickness, kTargetColor);
  DrawLineThick(p3_top, p4_top, kLineThickness, kTargetColor);
  DrawLineThick(p4_top, p1_top, kLineThickness, kTargetColor);

  DrawLineThick(p1_top, p1_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p2_top, p2_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p3_top, p3_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p4_top, p4_bottom, kLineThickness, kTargetColor);

  DrawLineThick(p1_bottom, p2_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p2_bottom, p3_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p3_bottom, p4_bottom, kLineThickness, kTargetColor);
  DrawLineThick(p4_bottom, p1_bottom, kLineThickness, kTargetColor);
}
