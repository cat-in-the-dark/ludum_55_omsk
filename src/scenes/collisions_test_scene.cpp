#include "collisions_test_scene.h"

#include "const.h"
#include "lib/collisions.h"

CollisionsTestScene::CollisionsTestScene()
    : triangle_{}, line_{}, circleCenter_{kWindowWidth / 2, kWindowHeight / 2}, circleRadius_{25.0f} {}

void CollisionsTestScene::Activate() {}

void CollisionsTestScene::Exit() {}

void CollisionsTestScene::Update() {
  auto pos = GetMousePosition();
  line_.first = pos;
  line_.second = pos + Vector2{100.0f, 100.0f};
  triangle_.p1 = pos;
  triangle_.p2 = pos + Vector2{-100.0f, -100.0f};
  triangle_.p3 = pos + Vector2{-150.0f, 100.0f};
}

void CollisionsTestScene::Draw() {
  ClearBackground(BLACK);
  DrawCircleLines(circleCenter_.x, circleCenter_.y, circleRadius_, RED);
  Color lineColor = RED;
  if (CheckCollisionLineCircle(line_.first, line_.second, circleCenter_, circleRadius_ * circleRadius_)) {
    lineColor = GREEN;
  }

  DrawLine(line_.first.x, line_.first.y, line_.second.x, line_.second.y, lineColor);

  Color triangleColor = RED;
  if (CheckCollisionTriangleCircle(triangle_, circleCenter_, circleRadius_)) {
    triangleColor = GREEN;
  }
  DrawTriangleLines(triangle_.p1, triangle_.p2, triangle_.p3, triangleColor);
}
