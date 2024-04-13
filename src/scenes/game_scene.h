#pragma once

#include <raylib.h>

#include <list>
#include <memory>
#include <vector>

#include "CatmullRom.h"
#include "entities/black_hole.h"
#include "entities/circle_wall.h"
#include "entities/player.h"
#include "entities/target.h"
#include "lib/scene.h"
#include "lib/tasks/timer.h"
#include "line.h"

struct GameWorld {
  Player player;
  std::vector<CircleWall> walls;
  std::vector<BlackHole> black_holes;
  Target target;
  Camera2D camera;
};

std::unique_ptr<GameWorld> createLevel1();

class GameScene : public Scene {
  std::unique_ptr<GameWorld> game_world;
  std::list<LineSystem> wave_systems;

 public:
  void Activate();
  void Exit();
  void Update();
  void Draw();
};
