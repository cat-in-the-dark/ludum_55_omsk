#pragma once

#include "lib/types.h"

class AntiCircleWall {
 public:
  Vector2 pos;
  float radius;

  AntiCircleWall(Vector2 pos, float radius);
  void Update();
  void Draw();
};
