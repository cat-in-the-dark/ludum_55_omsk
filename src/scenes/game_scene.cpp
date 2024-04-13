#include "game_scene.h"

#include <algorithm>

#include "balance.h"
#include "const.h"
#include "entities/player.h"
#include "lib/collisions.h"
#include "lib/renderer.h"
#include "lib/types.h"

void GameScene::Activate() {
  // TODO: generate other levels later
  game_world = createLevel1();
}

void GameScene::Exit() {}

void GameScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    wave_systems.emplace_back(SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime,
                                            balance::kWaveSegmentLifetime, balance::kWaveSpeed));
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

  game_world->camera.target = player.position;

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
        const auto& pos = particle.Pos();
        for (auto& cr : game_world->walls) {
          if (CheckCollisionPointCircle(pos, cr.pos, cr.radius)) {
            auto n = Vector2Normalize(cr.pos - pos);
            auto d = Vector2Refract(particle.dir, n, 1);
            particle.dir = d;
          }
        }
      }
    }
  }

  player.Update(dt);
  game_world->target.Update(dt);
}

void GameScene::Draw() {
  ClearBackground(BLACK);

  BeginMode2D(game_world->camera);
  for (auto& ws : wave_systems) {
    ws.Draw();
  }

  game_world->player.Draw();
  game_world->target.Draw();
  EndMode2D();
}

std::unique_ptr<GameWorld> createLevel1() {
  auto player = Player{{100.0f, 100.0f}};
  std::vector<CircleWall> circles = {{{500.0f, 0.0f}, 200.0f}};
  auto camera = Camera2D{{kWindowWidth / 2, kWindowHeight / 2}, player.position, 0.0f, 1.0f};
  auto res = GameWorld{player, circles, {}, {450.0f, 250.0f}, camera};
  return std::make_unique<GameWorld>(std::move(res));
}
