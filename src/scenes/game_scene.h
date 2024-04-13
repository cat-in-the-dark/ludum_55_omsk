#pragma once

#include <raylib.h>

#include <list>
#include <memory>
#include <vector>

#include "CatmullRom.h"
#include "entities/circle_wall.h"
#include "entities/player.h"
#include "lib/scene.h"
#include "lib/tasks/timer.h"
#include "wave.h"

namespace balance {
constexpr auto kWaveSpeed = 50.0f;
constexpr auto kWaveLifetime = 5.0f;
};  // namespace balance

struct GameWorld {
  Player player;
  std::vector<CircleWall> walls;
};

std::unique_ptr<GameWorld> createLevel1();

class GameScene : public Scene {
  Timer timer = {2};
  std::vector<Vector2> points = {{50, 50}, {55, 114}, {82, 140}, {114, 114}, {114, 50}};
  std::unique_ptr<Curve> curve = std::make_unique<CatmullRom>();
  std::unique_ptr<GameWorld> game_world;
  std::list<WaveSystem> wave_systems;

 public:
  void Activate();
  void Exit();
  void Update();
  void Draw();
};
