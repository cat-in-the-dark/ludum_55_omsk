#include "anti_wall.h"

#include <const.h>

AntiCircleWall::AntiCircleWall(Vector2 pos, float radius) : pos(pos), radius(radius) {}
void AntiCircleWall::Update(float dt) {}
void AntiCircleWall::Draw() {
  if (kDebugRender) {
    DrawCircleLines(pos.x, pos.y, radius, RAYWHITE);
  }
}
