#pragma once

#include <raylib.h>

#include <vector>

#include "../entities/black_hole.h"
#include "../entities/circle_wall.h"
#include "../wave.h"
#include "lib/scene.h"
#include "lib/types.h"

class TestRenderScene : public Scene {
  std::vector<WaveSystem> wave_systems;
  std::vector<BlackHole> black_holes;
  std::vector<CircleWall> circle_wall;

  Triangle2D tri;

  void CheckCollisions();

 public:
  TestRenderScene();
  void Activate();
  void Exit();
  void Update();
  void Draw();
};
