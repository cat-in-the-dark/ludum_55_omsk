#pragma once

#include <raylib.h>

#include <memory>
#include <vector>

#include "CatmullRom.h"
#include "entities/player.h"
#include "lib/scene.h"
#include "lib/tasks/timer.h"

struct GameWorld {
  Player player;
  std::vector<Circle> walls;
};

std::unique_ptr<GameWorld> createLevel1();

class GameScene : public Scene {
  Timer timer = {2};
  std::vector<Vector2> points = {{50, 50}, {55, 114}, {82, 140}, {114, 114}, {114, 50}};
  std::unique_ptr<Curve> curve = std::make_unique<CatmullRom>();
  std::unique_ptr<GameWorld> game_world;

 public:
  void Activate();
  void Exit();
  void Update();
  void Draw();
};
