#include "black_hole.h"

#include "../const.h"

BlackHole::BlackHole(Vector2 pos, float radius) : pos(pos), radius(radius) {}

void BlackHole::Update() {}

void BlackHole::Draw() {
  if (kDebugRender) {
    DrawCircleLines(pos.x, pos.y, radius, RAYWHITE);
  }
}
