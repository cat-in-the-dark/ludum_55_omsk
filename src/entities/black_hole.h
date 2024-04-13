#pragma once

#include "lib/types.h"

class BlackHole {
 public:
  Vector2 pos;
  float radius;

  BlackHole(Vector2 pos, float radius);
  void Update(float dt);
};
