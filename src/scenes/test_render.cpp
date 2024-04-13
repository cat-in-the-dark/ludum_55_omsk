#include "test_render.h"

#include "../const.h"

TestRenderScene::TestRenderScene()
    : tri({{0 + 300, 200 + 10.0f},
           {300 + -10 * float(std::sqrt(3)) / 2.0f, 200 + 25.0f},
           {300 + 10.0f * float(std::sqrt(3)) / 2.0f, 200 + 25.0f}}) {}
void TestRenderScene::Activate() {
  black_holes.emplace_back(Vector2{500, 300}, 32);
  circle_wall.emplace_back(Vector2{50, 100}, 128);
  circle_wall.emplace_back(Vector2{250, -32}, 128);
}
void TestRenderScene::Exit() {}
void TestRenderScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    wave_systems.emplace_back(SpawnTriangle(tri, 10, 120));
  }

  for (auto& ws : wave_systems) {
    ws.Update(GetFrameTime());
  }

  CheckCollisions();
}
void TestRenderScene::Draw() {
  ClearBackground(BLACK);
  if (kDebugRender) {
    DrawTriangle(tri.p1, tri.p2, tri.p3, RED);
  }

  for (auto& ws : wave_systems) {
    ws.Draw();
  }

  for (auto& bh : black_holes) {
    bh.Draw();
  }
  for (auto& cr : circle_wall) {
    cr.Draw();
  }
}

void TestRenderScene::CheckCollisions() {
  for (auto& ws : wave_systems) {
    for (auto& particle : ws.particles) {
      for (auto& bh : black_holes) {
        float dist = Vector2Distance(bh.pos, particle.pos);
        auto dir = Vector2Normalize(bh.pos - particle.pos);

        if (dist <= bh.radius / 2) {
          particle.alive = false;
          particle.pos = bh.pos;
          particle.dir = {0, 0};
        } else {
          auto pwr = 100.0 / (dist * dist);
          particle.dir = particle.dir + dir * pwr;
        }
      }
      for (auto& cr : circle_wall) {
        if (CheckCollisionPointCircle(particle.pos, cr.pos, cr.radius)) {
          auto n = Vector2Normalize(cr.pos - particle.pos);
          auto d = Vector2Refract(particle.dir, n, 1);
          particle.dir = d;
        }
      }
    }
  }
}
