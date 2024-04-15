#include "circle_wall.h"

#include "../const.h"

CircleWall::CircleWall(Vector2 pos, float radius) : pos(pos), radius(radius) {}

void CircleWall::Update() {}

void CircleWall::Draw() {
  if (kDebugRender) {
    DrawCircleLines(pos.x, pos.y, radius, RAYWHITE);
  }
}
