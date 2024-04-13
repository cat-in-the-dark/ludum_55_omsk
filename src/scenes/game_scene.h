#pragma once

#include <raylib.h>

#include <list>
#include <memory>
#include <vector>

#include "CatmullRom.h"
#include "const.h"
#include "entities/black_hole.h"
#include "entities/circle_wall.h"
#include "entities/player.h"
#include "entities/target.h"
#include "lib/scene.h"
#include "lib/scene_manager.h"
#include "line.h"

struct GameWorld {
  GameWorld(Player player, std::vector<CircleWall>&& circle_walls, std::vector<BlackHole>&& black_holes, Target target)
      : player{player},
        circle_walls{std::move(circle_walls)},
        line_systems{},
        black_holes{std::move(black_holes)},
        target{target} {
    camera = {{kCanvasWidth / 2, kCanvasHeight / 2}, player.position, 0.0f, 1.0f};
  }
  Player player;
  std::vector<CircleWall> circle_walls;
  std::list<LineSystem> line_systems;
  std::vector<BlackHole> black_holes;
  Target target;
  Camera2D camera;
};

std::unique_ptr<GameWorld> createLevel1();

class GameScene : public Scene {
  std::unique_ptr<GameWorld> game_world;
  size_t current_level = 0;
  std::vector<std::function<std::unique_ptr<GameWorld>()>> level_creators = {createLevel1};

 public:
  GameScene(SceneManager* sm) : sm_(sm) {}
  void Activate();
  void Exit();
  void Update();
  void Draw();

 private:
  void MovePlayer(float dt);
  void CheckCollisions();
  SceneManager* sm_;
};
