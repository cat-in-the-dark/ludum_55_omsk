#include "test_render.h"

#include "../const.h"
#include "balance.h"
#include "collision_system.h"

TestRenderScene::TestRenderScene()
    : tri({{0 + 300, 200 + 10.0f},
           {300 + -10 * float(std::sqrt(3)) / 2.0f, 200 + 25.0f},
           {300 + 10.0f * float(std::sqrt(3)) / 2.0f, 200 + 25.0f}}),
      anti_wall({320, 180}, 250) {}
void TestRenderScene::Activate() {
  black_holes.emplace_back(Vector2{500, 300}, 32);
  circle_wall.emplace_back(Vector2{50, 100}, 128);
  circle_wall.emplace_back(Vector2{250, -32}, 128);
}
void TestRenderScene::Exit() {}
void TestRenderScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    line_systems.emplace_back(SpawnTriangle(tri, balance::kWaveLifetime, balance::kWaveSegmentLifetime, 3));
    line_systems.emplace_back(
        SpawnCircle(Circle{Vector2{50, 100}, 128}, balance::kWaveLifetime, balance::kWaveSegmentLifetime, 3, 36));
  }

  anti_wall.Update();
  for (auto& ls : line_systems) {
    ls.Update(GetFrameTime());
  }

  CheckCollisions();
}
void TestRenderScene::Draw() {
  ClearBackground(BLACK);
  DrawTriangle(tri.p1, tri.p2, tri.p3, kPlayerColor);

  anti_wall.Draw();

  for (auto& bh : black_holes) {
    bh.Draw();
  }
  for (auto& cr : circle_wall) {
    cr.Draw();
  }

  for (auto& ls : line_systems) {
    ls.Draw();
  }
}

void TestRenderScene::CheckCollisions() {
  for (auto& ls : line_systems) {
    for (auto& particle : ls.particles) {
      CheckCollisionAntiWall(anti_wall, particle);

      CheckCollisionBlackHole(black_holes, particle);

      CheckCollisionCircleWalls(circle_wall, particle);
    }
  }
}
