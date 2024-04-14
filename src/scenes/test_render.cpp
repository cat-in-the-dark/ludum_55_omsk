#include "test_render.h"

#include "../const.h"
#include "balance.h"

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
  }

  anti_wall.Update(GetFrameTime());
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

void CheckCollisionAntiWall(const AntiCircleWall& wall, Line& line) {
  const auto& pos = line.Pos();

  if (!CheckCollisionPointCircle(pos, wall.pos, wall.radius)) {
    auto n = Vector2Normalize(pos - wall.pos);
    auto d = Vector2Refract(line.dir, n, 1);
    line.dir = d;
  }
}

void TestRenderScene::CheckCollisions() {
  for (auto& ls : line_systems) {
    for (auto& particle : ls.particles) {
      const auto& pos = particle.Pos();

      CheckCollisionAntiWall(anti_wall, particle);

      for (auto& bh : black_holes) {
        float dist = Vector2Distance(bh.pos, pos);
        auto dir = Vector2Normalize(bh.pos - pos);

        if (dist <= bh.radius / 2) {
          particle.Stop();
        } else {
          auto pwr = 100.0 / (dist * dist);
          particle.dir = particle.dir + dir * pwr;
        }
      }
      for (auto& cr : circle_wall) {
        if (CheckCollisionPointCircle(pos, cr.pos, cr.radius)) {
          auto n = Vector2Normalize(cr.pos - pos);
          auto d = Vector2Refract(particle.dir, n, 1);
          particle.dir = d;
        }
      }
    }
  }
}
