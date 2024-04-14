#include "enemy.h"

#include "const.h"

void Enemy::Draw() {
  if (kDebugRender) {
    DrawCircleLines(shape.center.x, shape.center.y, shape.radius, RAYWHITE);
  }
}

void Enemy::Update(const Player& player, float dt) {
  if (activated) {
    auto spd = Vector2Normalize(player.position - shape.center) * speed;
    shape.center = shape.center + spd;
    cooldown.Update(dt);
  }
}
