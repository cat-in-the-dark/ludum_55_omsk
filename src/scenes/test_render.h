#pragma once

#include <raylib.h>

#include <vector>

#include "entities/anti_wall.h"
#include "entities/black_hole.h"
#include "entities/circle_wall.h"
#include "lib/scene.h"
#include "lib/types.h"
#include "line.h"

class TestRenderScene : public Scene {
  std::vector<LineSystem> line_systems;
  std::vector<BlackHole> black_holes;
  std::vector<CircleWall> circle_wall;
  Triangle2D tri;
  AntiCircleWall anti_wall;

  void CheckCollisions();

 public:
  TestRenderScene();
  void Activate();
  void Exit();
  void Update();
  void Draw();
};
