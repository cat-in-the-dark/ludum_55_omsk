#pragma once

#include "entity.h"
#include "lib/types.h"

constexpr auto kTargetSize = 30.0f;

class Target : public Entity {
 public:
  Target(float x, float y) : pos{x, y}, angle_{0.0f} {}
  void Update(float dt);
  void Draw();

  Vector2 pos;

 private:
  float angle_;
};
