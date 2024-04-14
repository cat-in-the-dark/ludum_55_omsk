#include "game_scene.h"

#include <algorithm>

#include "balance.h"
#include "collision_system.h"
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
  auto dt = GetFrameTime();

  const auto epsilon = 0.0001f;
  auto& player = game_world->player;
  auto speed = MovePlayer();
  if (Vector2LengthSqr(speed) > epsilon) {
    player.position = player.position + speed;
    game_world->wave_cooldown.Update(dt);
  }

  auto& line_systems = game_world->line_systems;

  if (game_world->wave_cooldown.Invoke()) {
    line_systems.emplace_back(SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime,
                                            balance::kWaveSegmentLifetime, balance::kWaveSpeed));
  }

  game_world->camera.target = player.position;

  for (auto& line : line_systems) {
    line.Update(dt);
  }

  for (auto& enemy : game_world->enemies) {
    enemy.Update(player, dt);
    if (enemy.cooldown.Invoke()) {
      line_systems.emplace_back(SpawnCircle(enemy.shape, balance::kWaveLifetime, balance::kWaveSegmentLifetime,
                                            balance::kWaveSpeed, balance::kEnemyLinesCount));
    }
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

  for (auto& cw : game_world->circle_walls) {
    cw.Draw();
  }

  for (auto& enemy : game_world->enemies) {
    enemy.Draw();
  }

  game_world->anti_wall.Draw();

  game_world->player.Draw();
  game_world->target.Draw();
  EndMode2D();
}

std::unique_ptr<GameWorld> createLevel1() {
  auto player = Player{{100.0f, 100.0f}};
  std::vector<CircleWall> circles = {{{500.0f, 0.0f}, 200.0f}};
  std::vector<BlackHole> black_holes = {{{30.0f, 320.0f}, 30.0f}};
  std::vector<Enemy> enemies = {{{320, 300}, 25}};
  auto anti_wall = AntiCircleWall{{320, 180}, 250};
  auto res =
      GameWorld{player, std::move(circles), std::move(black_holes), std::move(enemies), anti_wall, {450.0f, 250.0f}};
  return std::make_unique<GameWorld>(std::move(res));
}

Vector2 GameScene::MovePlayer() {
  Vector2 speed{};
  if (IsKeyDown(KEY_DOWN)) {
    speed.y = 1;
  }
  if (IsKeyDown(KEY_UP)) {
    speed.y = -1;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    speed.x = 1;
  }
  if (IsKeyDown(KEY_LEFT)) {
    speed.x = -1;
  }

  return Vector2Normalize(speed) * balance::kPlayerSpeed;
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

  CheckCollisionsPlayerCircleWalls(game_world->circle_walls, game_world->player);

  CheckCollisionsPlayerAntiWall(game_world->anti_wall, game_world->player);

  for (auto& enemy : game_world->enemies) {
    CheckCollisionEnemyCircleWalls(game_world->circle_walls, enemy);
  }

  for (auto& ls : game_world->line_systems) {
    for (auto& particle : ls.particles) {
      CheckCollisionEnemies(game_world->enemies, particle);

      CheckCollisionBlackHole(game_world->black_holes, particle);

      CheckCollisionCircleWalls(game_world->circle_walls, particle);

      CheckCollisionAntiWall(game_world->anti_wall, particle);
    }
  }
}
