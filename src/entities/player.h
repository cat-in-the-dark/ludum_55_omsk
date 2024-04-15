#pragma once

#include <raylib.h>

#include <memory>

#include "lib/types.h"

constexpr auto kPlayerSize = 15.0f;

class Player {
 public:
  Player(Vector2&& position) : shape{createPlayerShape(kPlayerSize)}, position(std::move(position)) {}
  void Update();
  void Draw();

  Triangle2D shape;
  Vector2 position;
  Color color{GREEN};

  Triangle2D GetPlayerShape() {
    return {shape.p1 + position, shape.p2 + position, shape.p3 + position};
  }

 private:
  Triangle2D createPlayerShape(float radius) {
    auto vec = Vector2{0, -radius};
    // angles move clockwise...
    auto p1 = Vector2Scale(vec, 2.0f);
    auto p2 = Vector2Rotate(vec, 4 * PI / 3);
    auto p3 = Vector2Rotate(vec, 2 * PI / 3);
    return {p1, p2, p3};
  }
};
