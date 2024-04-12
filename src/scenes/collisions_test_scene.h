#pragma once

#include <raylib.h>

#include <utility>

#include "lib/scene.h"
#include "lib/types.h"

class CollisionsTestScene : public Scene {
 public:
  CollisionsTestScene();
  void Activate();
  void Exit();
  void Update();
  void Draw();

 private:
  Triangle2D triangle_;
  std::pair<Vector2, Vector2> line_;
  Vector2 circleCenter_;
  float circleRadius_;
};
