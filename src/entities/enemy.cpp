#include "enemy.h"

#include "const.h"

void Enemy::Draw() {
  if (kDebugRender) {
    DrawCircleLines(shape.center.x, shape.center.y, shape.radius, RAYWHITE);
  }
}

void Enemy::Update(const Player& player, float dt) {
  if (activated) {
    auto speed = Vector2Normalize(player.position - shape.center) * balance::kEnemySpeed;
    shape.center = shape.center + speed;
    cooldown.Update(dt);
  }
}
