#pragma once

#include "../lib/types.h"

class CircleWall {
 public:
  Vector2 pos;
  float radius;

  CircleWall(Vector2 pos, float radius);
  void Update(float dt);
};
