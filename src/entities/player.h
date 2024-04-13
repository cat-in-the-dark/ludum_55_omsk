#pragma once

#include <raylib.h>

#include <memory>

#include "entity.h"
#include "lib/types.h"

constexpr auto kPlayerSize = 15.0f;

class Player : public Entity {
  Color color_{GREEN};
  Triangle2D shape_;

 public:
  Player(Vector2&& position) : shape_{createPlayerShape(kPlayerSize)}, position(std::move(position)) {}
  void Update(float dt);
  void Draw();

  Vector2 position;

 private:
  Triangle2D createPlayerShape(float radius) {
    auto vec = Vector2{0, -radius};
    auto p1 = Vector2Scale(vec, 2.0f);
    auto p2 = Vector2Rotate(vec, 2 * PI / 3);
    auto p3 = Vector2Rotate(vec, 4 * PI / 3);
    return {p1, p2, p3};
  }

  Triangle2D getPlayerShape() {
    return {shape_.p1 + position, shape_.p2 + position, shape_.p3 + position};
  }
};
