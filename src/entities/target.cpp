#include "target.h"

#include <raylib.h>

constexpr auto kTargetColor = LIME;
constexpr auto kRotationSpeed = PI / 4;

void Target::Update(float dt) {
  angle_ += kRotationSpeed * dt;
}

void Target::Draw() {
  DrawRectangleLines(pos.x, pos.y, kTargetSize, kTargetSize, kTargetColor);
  Vector2 line = Vector2Rotate({0.0f, kTargetSize}, angle_);
  auto end = pos + line;
  DrawLine(pos.x, pos.y, end.x, end.y, kTargetColor);
}
