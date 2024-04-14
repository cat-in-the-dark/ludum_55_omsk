#pragma once

#include "entities/player.h"
#include "lib/tasks/cooldown.h"
#include "lib/types.h"
#include "scenes/balance.h"

struct Enemy {
  Enemy(Vector2 pos, float radius) : shape{pos, radius} {}
  void Draw();
  void Update(const Player& player, float dt);
  Circle shape;
  bool activated = false;
  Cooldown cooldown = {balance::kEnemyWaveSpawnCooldown};
};
