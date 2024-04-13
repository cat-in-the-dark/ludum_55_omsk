#pragma once

#include "entity.h"
#include "lib/types.h"

class Target : public Entity {
 public:
  Target(float x, float y) : pos{x, y}, angle_{0.0f} {}
  void Update(float dt);
  void Draw();

  Vector2 pos;

 private:
  float angle_;
};
