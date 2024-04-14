#include "collision_system.h"

#include "const.h"
#include "lib/collisions.h"

void CheckCollisionAntiWall(const AntiCircleWall& wall, Line& line) {
  const auto& pos = line.Pos();

  if (!CheckCollisionPointCircle(pos, wall.pos, wall.radius)) {
    auto n = Vector2Normalize(pos - wall.pos);
    auto d = Vector2Refract(line.dir, n, 1);
    line.dir = d;
  }
}

void CheckCollisionBlackHole(const std::vector<BlackHole>& black_holes, Line& line) {
  const auto& pos = line.Pos();

  for (auto& bh : black_holes) {
    float dist = Vector2Distance(bh.pos, pos);
    auto dir = Vector2Normalize(bh.pos - pos);

    if (dist <= bh.radius / 2) {
      line.Stop();
    } else {
      auto pwr = 100.0 / (dist * dist);
      line.dir = line.dir + dir * pwr;
    }
  }
}

void CheckCollisionCircleWalls(const std::vector<CircleWall>& circle_walls, Line& line) {
  const auto& pos = line.Pos();

  for (auto& cr : circle_walls) {
    if (CheckCollisionPointCircle(pos, cr.pos, cr.radius)) {
      auto n = Vector2Normalize(cr.pos - pos);
      auto d = Vector2Refract(line.dir, n, 1);
      line.dir = d;
    }
  }
}

void CheckCollisionEnemyCircleWalls(const std::vector<CircleWall>& circle_walls, Enemy& enemy) {
  auto& pos = enemy.shape.center;
  for (auto& wall : circle_walls) {
    if (CheckCollisionCircles(pos, kPlayerSize, wall.pos, wall.radius)) {
      auto dir = Vector2Normalize(pos - wall.pos);
      pos = wall.pos + Vector2Scale(dir, kPlayerSize + wall.radius);
    }
  }
}

void CheckCollisionEnemies(std::vector<Enemy>& enemies, Line& line) {
  const auto& pos = line.Pos();

  for (auto& enemy : enemies) {
    if (CheckCollisionPointCircle(pos, enemy.shape.center, enemy.shape.radius)) {
      enemy.activated = true;
      line.color = kEnemyColor;
      auto n = Vector2Normalize(enemy.shape.center - pos);
      auto d = Vector2Refract(line.dir, n, 1);
      line.dir = d;
    }
  }
}

void CheckCollisionsPlayerCircleWalls(const std::vector<CircleWall>& circle_walls, Player& player) {
  auto& pos = player.position;
  for (auto& wall : circle_walls) {
    if (CheckCollisionCircles(pos, kPlayerSize, wall.pos, wall.radius)) {
      auto dir = Vector2Normalize(pos - wall.pos);
      pos = wall.pos + Vector2Scale(dir, kPlayerSize + wall.radius);
    }
  }
}

void CheckCollisionsPlayerAntiWall(const AntiCircleWall& anti_wall, Player& player) {
  auto& pos = player.position;
  if (!CircleInCircle(anti_wall.pos, anti_wall.radius, pos, kPlayerSize)) {
    auto dir = Vector2Normalize(pos - anti_wall.pos);
    pos = anti_wall.pos + Vector2Scale(dir, anti_wall.radius - kPlayerSize);
  }
}
