#include "game_scene.h"

#include <algorithm>

#include "entities/player.h"
#include "lib/collisions.h"
#include "lib/renderer.h"
#include "lib/types.h"

void GameScene::Activate() {
  // TODO: generate other levels later
  game_world = createLevel1();

  curve->set_steps(20);
  curve->clear();
  auto& first = *points.begin();
  auto& last = *points.rbegin();
  curve->add_way_point({last.x, last.y, 0.0});
  for (auto& p : points) {
    curve->add_way_point({p.x, p.y, 0.0});
  }
  curve->add_way_point({first.x, first.y, 0.0});
}

void GameScene::Exit() {}

void GameScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    wave_systems.emplace_back(
        SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime, balance::kWaveSpeed));
  }

  auto dt = GetFrameTime();
  const auto player_speed = 100.0f * dt;

  auto& player = game_world->player;
  if (IsKeyDown(KEY_DOWN)) {
    player.position.y += player_speed;
  } else if (IsKeyDown(KEY_UP)) {
    player.position.y -= player_speed;
  } else if (IsKeyDown(KEY_RIGHT)) {
    player.position.x += player_speed;
  } else if (IsKeyDown(KEY_LEFT)) {
    player.position.x -= player_speed;
  }

  for (auto& wave : wave_systems) {
    wave.Update(dt);
  }

  wave_systems.erase(
      std::remove_if(wave_systems.begin(), wave_systems.end(), [](const auto& ws) { return !ws.Alive(); }),
      wave_systems.end());

  // TODO: move to the separate collision-checking function
  auto& pos = player.position;
  for (auto& wall : game_world->walls) {
    if (CheckCollisionCircles(pos, kPlayerSize, wall.pos, wall.radius)) {
      auto dir = Vector2Normalize(pos - wall.pos);
      pos = wall.pos + Vector2Scale(dir, kPlayerSize + wall.radius);
    }
    for (auto& ws : wave_systems) {
      for (auto& particle : ws.particles) {
        for (auto& cr : game_world->walls) {
          if (CheckCollisionPointCircle(particle.pos, cr.pos, cr.radius)) {
            auto n = Vector2Normalize(cr.pos - particle.pos);
            auto d = Vector2Refract(particle.dir, n, 1);
            particle.dir = d;
          }
        }
      }
    }
  }

  player.Update(dt);
  timer.Update(dt);
}

void GameScene::Draw() {
  ClearBackground(BLACK);
  if (timer.IsPassed()) {
    DrawRectangle(40, 64, 100, 64, BLUE);
  } else {
    DrawRectangle(40, 64, 100, 64, GREEN);
  }

  for (auto& ws : wave_systems) {
    ws.Draw();
  }

  DrawLines(points, 5.0f, BLUE);
  std::vector<Vector2> outPoints;
  for (int i = 0; i < curve->node_count(); ++i) {
    auto&& point = curve->node(i);
    outPoints.push_back({static_cast<float>(point.x), static_cast<float>(point.y)});
  }
  DrawLines(outPoints, 5.0f, GREEN);

  // DEBUG ONLY
  game_world->player.Draw();
}

std::unique_ptr<GameWorld> createLevel1() {
  auto player = Player{{100.0f, 100.0f}};
  std::vector<CircleWall> circles = {{{500.0f, 300.0f}, 200.0f}};
  auto res = GameWorld{player, circles};
  return std::make_unique<GameWorld>(std::move(res));
}
