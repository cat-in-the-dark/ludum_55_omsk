#pragma once

#include <vector>

#include "lib/types.h"

class WaveParticle {
 public:
  Vector2 pos;
  Vector2 dir;
  Vector2 speed;

  WaveParticle(Vector2 pos, Vector2 dir, Vector2 speed);
  void Update(float dt);
};

class WaveSystem {
  std::vector<WaveParticle> particles;
  std::vector<Vector2> points;  // которые генерит spline
  float lifetime;
  float elapsed;

 public:
  WaveSystem(float lifetime);
  void AddParticle(WaveParticle particle);

  void Update(float dt);
  void Draw();
  bool Alive() const;
};

WaveSystem SpawnTriangle(Triangle2D tri, float lifetime);
