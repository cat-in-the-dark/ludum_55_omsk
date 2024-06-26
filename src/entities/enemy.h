#pragma once

#include "entities/player.h"
#include "lib/tasks/cooldown.h"
#include "lib/types.h"
#include "scenes/balance.h"

struct Enemy {
  Enemy(Vector2 pos, float radius, float speed) : shape{pos, radius}, speed(speed) {}
  Enemy(Vector2 pos, float radius, float speed, float cooldownTime)
      : shape{pos, radius}, speed(speed), cooldown{cooldownTime, cooldownTime * 0.99f} {}
  void Draw();
  void Update(const Player& player, float dt);
  Circle shape;
  float speed;

  // TODO: Learn how to delete elements from vectors!
  bool alive = true;
  bool activated = false;
  Cooldown cooldown = {balance::kEnemyWaveSpawnCooldown, balance::kEnemyWaveSpawnCooldown * 0.99f};
};
