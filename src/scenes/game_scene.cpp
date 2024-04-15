#include "game_scene.h"

#include <algorithm>

#include "balance.h"
#include "collision_system.h"
#include "const.h"
#include "entities/player.h"
#include "lib/collisions.h"
#include "lib/math.h"
#include "lib/renderer.h"
#include "lib/types.h"

void GameScene::Activate() {
  game_world = level_creators[current_level]();
}

void GameScene::Exit() {}

void GameScene::Update() {
  if (IsKeyPressed(KEY_TAB)) {
    TraceLog(LOG_INFO, "pos [%f, %f]", game_world->player.position.x, game_world->player.position.y);
    game_world->circle_walls.emplace_back(game_world->player.position, 32);
  }

  auto dt = GetFrameTime();
  auto& line_systems = game_world->line_systems;

  for (auto& line : line_systems) {
    line.Update(dt);
  }

  line_systems.erase(
      std::remove_if(line_systems.begin(), line_systems.end(), [](const auto& ls) { return !ls.Alive(); }),
      line_systems.end());

  game_world->target.Update(dt);

  switch (game_world->world_state) {
    case WorldState::IN_GAME: {
      UpdateGame(dt);
      break;
    }
    case WorldState::DEATH_FROM_BLACK_HOLE: {
      UpdateBlackHoleDeathAnimation(dt);
      break;
    }
    case WorldState::DEATH_FROM_ENEMY: {
      UpdateEnemyDeathAnimation(dt);
      break;
    }
    case WorldState::WINNING: {
      UpdateWinAnimation(dt);
      break;
    }

    default:
      break;
  }
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

  for (auto& bh : game_world->black_holes) {
    bh.Draw();
  }

  for (auto& enemy : game_world->enemies) {
    if (enemy.alive) {
      enemy.Draw();
    }
  }

  game_world->anti_wall.Draw();

  game_world->player.Draw();
  game_world->target.Draw();
  EndMode2D();
}

std::unique_ptr<GameWorld> createLevel0() {
  auto player = Player{{100.0f, 100.0f}};
  std::vector<CircleWall> circles = {{{250.0f, 100.0f}, 50.0f}};
  std::vector<BlackHole> black_holes = {{{320.0f, 180.0f}, 30.0f}};
  std::vector<Enemy> enemies = {{{320, 300}, 25, balance::kEnemySpeed}};
  auto anti_wall = AntiCircleWall{{320, 180}, 250};
  auto res =
      GameWorld{player, std::move(circles), std::move(black_holes), std::move(enemies), anti_wall, {450.0f, 250.0f}};
  return std::make_unique<GameWorld>(std::move(res));
}

std::unique_ptr<GameWorld> createLevel2() {
  auto player = Player{{0, 0}};
  std::vector<CircleWall> circles = {
      {{-64.0f, -64.0f}, 48.0f}, {{140.0f, 140.0f}, 32.0f}, {{180.0f, 110.0f}, 32.0f},
      {{225.0f, 65.0f}, 32.0f},  {{164.0f, 256.0f}, 32.0f}, {{234.0f, -20.0f}, 32.0f},
      {{213.0f, 218.0f}, 32.0f}, {{0.0f, 135.0f}, 32.0f},   {{72.0f, 198.0f}, 32.0f},
  };
  AntiCircleWall anti_wall({0, 0}, 320);
  Target target(250, 120);
  auto res = GameWorld{player, std::move(circles), {}, {}, anti_wall, target};
  return std::make_unique<GameWorld>(std::move(res));
}
std::unique_ptr<GameWorld> createLevel1() {
  auto player = Player{{0, 0}};
  AntiCircleWall anti_wall({0, 0}, 320);
  Target target(280, 0);
  return std::make_unique<GameWorld>(GameWorld{player, {}, {}, {}, anti_wall, target});
}
std::unique_ptr<GameWorld> createLevel3() {
  auto player = Player{{0, 0}};
  AntiCircleWall anti_wall({0, 0}, 400);
  std::vector<CircleWall> circles = {{{320.0f, 0.0f}, 180.0f}, {{-320.0f, 0.0f}, 180.0f}, {{0.0f, -320.0f}, 180.0f},
                                     {{0.0f, 320.0f}, 180.0f}, {{-100, 100}, 32},         {{-100, -100}, 32},
                                     {{100, -100}, 32},        {{100, 100}, 32}};
  Target target(200, -300);
  return std::make_unique<GameWorld>(GameWorld{player, std::move(circles), {}, {}, anti_wall, target});
}

std::unique_ptr<GameWorld> createLevel4() {
  auto player = Player{{0, 0}};
  AntiCircleWall anti_wall({0, 0}, 320);
  std::vector<BlackHole> black_holes = {{{160.0f, 0.0f}, 32.0f}};
  Target target(210, 0);
  return std::make_unique<GameWorld>(GameWorld{player, {}, std::move(black_holes), {}, anti_wall, target});
}

std::unique_ptr<GameWorld> createLevel5() {
  auto player = Player{{0, 0}};
  AntiCircleWall anti_wall({0, 0}, 320);
  std::vector<BlackHole> black_holes = {
      {{120.0f, -64.0f}, 32.0f},
      {{240.0f, 0}, 32.0f},
      {{120.0f, 64.0f}, 32.0f},
  };
  Target target(280, -5);
  return std::make_unique<GameWorld>(GameWorld{player, {}, std::move(black_holes), {}, anti_wall, target});
}

std::unique_ptr<GameWorld> createLevel6() {
  auto player = Player{{0, 280}};
  AntiCircleWall anti_wall({0, 0}, 320);
  std::vector<CircleWall> circles = {{{-240, 115}, 64}, {{240, 115}, 64}};
  std::vector<BlackHole> black_holes = {
      {{0.0f, 160.0f}, 32.0f}, {{-130.0f, 80.0f}, 32.0f},  {{130.0f, 80.0f}, 32.0f},
      {{0.0f, 0.0f}, 64.0f},   {{-280.0f, -64.0f}, 64.0f}, {{280.0f, -64.0f}, 64.0f},
  };
  Target target(0, -280);
  return std::make_unique<GameWorld>(
      GameWorld{player, std::move(circles), std::move(black_holes), {}, anti_wall, target});
}

std::unique_ptr<GameWorld> createLevel7() {
  auto player = Player{{-300, 0}};
  AntiCircleWall anti_wall({0, 0}, 480);

  std::vector<CircleWall> circles;
  for (int i = 0; i < 13; i++) {
    circles.emplace_back(Vector2{-440 + i * 60.0f, -128.0f}, 32.0f);
    circles.emplace_back(Vector2{-440 + i * 60.0f, 128.0f}, 32.0f);
  }
  std::vector<BlackHole> black_holes;
  std::vector<Enemy> enemies = {
      {{-440, 0}, 25, balance::kEnemySpeed},
      {{-90, 430}, 25, balance::kEnemySpeed * 6, balance::kEnemyWaveSpawnCooldown / 2},
      {{-120, 400}, 25, balance::kEnemySpeed * 6, balance::kEnemyWaveSpawnCooldown / 2},
  };
  Target target(110, -430);
  return std::make_unique<GameWorld>(
      GameWorld{player, std::move(circles), std::move(black_holes), std::move(enemies), anti_wall, target});
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
    game_world->world_state = WorldState::WINNING;
  }

  CheckCollisionsPlayerCircleWalls(game_world->circle_walls, game_world->player);

  CheckCollisionsPlayerAntiWall(game_world->anti_wall, game_world->player);

  CheckCollisionEnemiesBleckHoles(game_world->black_holes, game_world->enemies);

  for (auto& black_hole : game_world->black_holes) {
    if (CheckCollisionCircles(pos, kPlayerSize, black_hole.pos, black_hole.radius)) {
      game_world->hit_pos = black_hole.pos;
      game_world->player_hit_pos = game_world->player.position;
      game_world->world_state = WorldState::DEATH_FROM_BLACK_HOLE;
    }
  }

  for (auto& enemy : game_world->enemies) {
    if (!enemy.alive) {
      continue;
    }

    if (CheckCollisionCircles(pos, kPlayerSize, enemy.shape.center, enemy.shape.radius)) {
      game_world->world_state = WorldState::DEATH_FROM_ENEMY;
    }

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

void GameScene::UpdateGame(float dt) {
  const auto epsilon = 0.0001f;
  auto& player = game_world->player;
  auto speed = MovePlayer();
  player.position = player.position + speed;

  auto& line_systems = game_world->line_systems;

  game_world->wave_cooldown.Update(dt);
  if (Vector2LengthSqr(speed) > epsilon) {
    if (game_world->wave_cooldown.Invoke()) {
      line_systems.emplace_back(SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime,
                                              balance::kWaveSegmentLifetime, balance::kWaveSpeed));
    }
  }

  for (auto& enemy : game_world->enemies) {
    if (!enemy.alive) {
      continue;
    }

    enemy.Update(player, dt);
    if (enemy.cooldown.Invoke()) {
      line_systems.emplace_back(SpawnCircle(enemy.shape, balance::kWaveLifetime, balance::kWaveSegmentLifetime,
                                            balance::kWaveSpeed, balance::kEnemyLinesCount));
    }
  }

  game_world->camera.target = player.position;

  CheckCollisions();

  player.Update();
}

void GameScene::UpdateBlackHoleDeathAnimation(float dt) {
  game_world->death_timer.Update(dt);

  auto target_zoom = 5.0f;
  auto init_zoom = 1.0f;
  auto percent = Remap(game_world->death_timer.Elapsed(), 0.0f, kDeathTimeout, 0.0f, 1.0f);
  auto cubic_percent = easeInCubic(percent);
  auto zoom = Remap(cubic_percent, 0.0f, 1.0f, init_zoom, target_zoom);

  auto init_shake = 1.0f;
  auto max_shake = 10.0f;
  auto shake_amount = Remap(cubic_percent, 0.0f, 1.0f, init_shake, max_shake);
  auto random_dir = GetRandomValue(0, 360) * PI / 180;
  auto shake_vector = Vector2Rotate({0, shake_amount}, random_dir);

  auto current_pos = Lerp2D(game_world->player_hit_pos, game_world->hit_pos, cubic_percent);
  auto& player = game_world->player;
  auto& player_pos = game_world->player.position;
  player_pos = current_pos;

  auto init_scale = 1.0f;
  auto end_scale = 0.1f;
  auto scale_amount = Remap(cubic_percent, 0.0f, 1.0f, init_scale, end_scale);

  auto init_rotation = 0.0f;
  auto end_rotation = 4 * PI;
  auto rotation_amount = Remap(cubic_percent, 0.0f, 1.0f, init_rotation, end_rotation);

  auto p1 = Vector2Rotate(game_world->player_init_shape.p1 * scale_amount, rotation_amount);
  auto p2 = Vector2Rotate(game_world->player_init_shape.p2 * scale_amount, rotation_amount);
  auto p3 = Vector2Rotate(game_world->player_init_shape.p3 * scale_amount, rotation_amount);

  player.shape.p1 = p1;
  player.shape.p2 = p2;
  player.shape.p3 = p3;

  auto init_r = kPlayerColor.r;
  auto init_g = kPlayerColor.g;
  auto end_r = 255;
  auto end_g = 0;

  auto current_r = static_cast<unsigned char>(Clamp(Remap(cubic_percent, 0.0f, 1.0f, init_r, end_r), 0.0f, 255.0f));
  auto current_g = static_cast<unsigned char>(Clamp(Remap(cubic_percent, 0.0f, 1.0f, init_g, end_g), 0.0f, 255.0f));

  auto& p_color = game_world->player.color;
  p_color.r = current_r;
  p_color.g = current_g;

  game_world->camera.zoom = zoom;
  game_world->camera.offset = game_world->camera.offset + shake_vector;
  game_world->camera.rotation += 0.5f;

  if (game_world->death_timer.IsPassed()) {
    sm_->Change("gameover");
  }
}

void GameScene::UpdateEnemyDeathAnimation(float dt) {
  game_world->death_timer.Update(dt);

  auto target_zoom = 3.0f;
  auto init_zoom = 1.0f;
  auto percent = Remap(game_world->death_timer.Elapsed(), 0.0f, kDeathTimeout, 0.0f, 1.0f);
  auto cubic_percent = easeInCubic(percent);
  auto zoom = Remap(cubic_percent, 0.0f, 1.0f, init_zoom, target_zoom);

  auto init_shake = 1.0f;
  auto max_shake = 10.0f;
  auto shake_amount = Remap(cubic_percent, 0.0f, 1.0f, init_shake, max_shake);
  auto random_dir = GetRandomValue(0, 360) * PI / 180;
  auto shake_vector = Vector2Rotate({0, shake_amount}, random_dir);

  auto player_pos = game_world->player.position;

  auto& p_color = game_world->player.color;
  const auto color_speed = 6;
  p_color.r = Clamp(p_color.r + color_speed, 0.0f, 255.0f);
  p_color.g = Clamp(p_color.g - color_speed, 0.0f, 255.0f);

  game_world->camera.zoom = zoom;
  game_world->camera.target = player_pos + shake_vector;

  player_pos = player_pos + shake_vector;

  auto& pos = game_world->player.position;
  auto dir = Vector2Normalize(game_world->hit_pos - pos);
  pos = pos + dir * (balance::kPlayerSpeed / 2);

  if (game_world->death_timer.IsPassed()) {
    sm_->Change("gameover");
  }
}

void GameScene::UpdateWinAnimation(float dt) {
  game_world->win_timer.Update(dt);
  game_world->win_spawn_timer.Update(dt);
  if (game_world->win_spawn_timer.IsPassed()) {
    game_world->line_systems.emplace_back(SpawnTriangle(game_world->player.GetPlayerShape(), balance::kWaveLifetime,
                                                        balance::kWaveSegmentLifetime, balance::kWaveSpeed));
  }

  auto target_zoom = 3.0f;
  auto init_zoom = 1.0f;
  auto percent = Remap(game_world->win_timer.Elapsed(), 0.0f, kWinTimeout, 0.0f, 1.0f);
  auto cubicPercent = easeOutCubic(percent);
  auto zoom = Remap(cubicPercent, 0.0f, 1.0f, init_zoom, target_zoom);

  game_world->camera.zoom = zoom;
  if (game_world->win_timer.IsPassed()) {
    current_level++;
    if (current_level >= level_creators.size()) {
      current_level = 0;
      sm_->Change("gamewin");
    } else {
      sm_->Change("next");
    }
  }
}
