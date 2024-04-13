#include "game_scene.h"

#include <algorithm>

#include "balance.h"
#include "const.h"
#include "entities/player.h"
#include "lib/collisions.h"
#include "lib/renderer.h"
#include "lib/types.h"

void GameScene::Activate() {
  game_world = level_creators[current_level]();
}

void GameScene::Exit() {}

void GameScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    game_world->line_systems.emplace_back(SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime,
                                                        balance::kWaveSegmentLifetime, balance::kWaveSpeed));
  }

  auto dt = GetFrameTime();

  auto& player = game_world->player;
  MovePlayer(dt);
  game_world->camera.target = player.position;

  auto& line_systems = game_world->line_systems;
  for (auto& line : line_systems) {
    line.Update(dt);
  }

  line_systems.erase(
      std::remove_if(line_systems.begin(), line_systems.end(), [](const auto& ls) { return !ls.Alive(); }),
      line_systems.end());

  CheckCollisions();

  player.Update(dt);
  game_world->target.Update(dt);
}

void GameScene::Draw() {
  ClearBackground(BLACK);

  BeginMode2D(game_world->camera);
  for (auto& ls : game_world->line_systems) {
    ls.Draw();
  }

  game_world->player.Draw();
  game_world->target.Draw();
  EndMode2D();
}

std::unique_ptr<GameWorld> createLevel1() {
  auto player = Player{{100.0f, 100.0f}};
  std::vector<CircleWall> circles = {{{500.0f, 0.0f}, 200.0f}};
  auto res = GameWorld{player, std::move(circles), {}, {450.0f, 250.0f}};
  return std::make_unique<GameWorld>(std::move(res));
}

void GameScene::MovePlayer(float dt) {
  auto& player = game_world->player;

  const auto player_speed = 100.0f * dt;

  if (IsKeyDown(KEY_DOWN)) {
    player.position.y += player_speed;
  } else if (IsKeyDown(KEY_UP)) {
    player.position.y -= player_speed;
  } else if (IsKeyDown(KEY_RIGHT)) {
    player.position.x += player_speed;
  } else if (IsKeyDown(KEY_LEFT)) {
    player.position.x -= player_speed;
  }
}

void GameScene::CheckCollisions() {
  auto& pos = game_world->player.position;
  auto& target = game_world->target;
  auto targetRec = Rectangle{target.pos.x, target.pos.y, kTargetSize, kTargetSize};
  if (CheckCollisionCircleRec(pos, kPlayerSize, targetRec)) {
    current_level++;
    if (current_level >= level_creators.size()) {
      current_level = 0;
      sm_->Change("win");
    } else {
      sm_->Change("next");
    }
  }

  for (auto& wall : game_world->circle_walls) {
    if (CheckCollisionCircles(pos, kPlayerSize, wall.pos, wall.radius)) {
      auto dir = Vector2Normalize(pos - wall.pos);
      pos = wall.pos + Vector2Scale(dir, kPlayerSize + wall.radius);
    }
  }

  for (auto& ls : game_world->line_systems) {
    for (auto& particle : ls.particles) {
      const auto& pos = particle.Pos();

      for (auto& bh : game_world->black_holes) {
        float dist = Vector2Distance(bh.pos, pos);
        auto dir = Vector2Normalize(bh.pos - pos);

        if (dist <= bh.radius / 2) {
          particle.Stop();
        } else {
          auto pwr = 100.0 / (dist * dist);
          particle.dir = particle.dir + dir * pwr;
        }
      }
      for (auto& cr : game_world->circle_walls) {
        if (CheckCollisionPointCircle(pos, cr.pos, cr.radius)) {
          auto n = Vector2Normalize(cr.pos - pos);
          auto d = Vector2Refract(particle.dir, n, 1);
          particle.dir = d;
        }
      }
    }
  }
}
