#pragma once

#include <vector>

#include "lib/types.h"

class Segment {
 public:
  Vector2 p1;
  Vector2 p2;
  float elapsed;
  float lifetime;
  Color color = RED;

  Segment(Vector2 p1, Vector2 p2, float lifetime);

  float Length() const;
  bool Alive() const {
    return elapsed <= lifetime;
  }
};

class Line {
  std::vector<Segment> segments;

 public:
  Vector2 dir;
  float speed;
  float elapsed = 0;
  float lifetime;
  float segment_lifetime;

  float max_segment_len = 5;

  Line(Vector2 start, Vector2 dir, float speed, float lifetime, float segment_lifetime);
  void Update(float dt);
  void Draw();
  bool Alive() const {
    return elapsed <= lifetime;
  }

  void SpawnSegment();
  const Vector2& Pos() const;

  void Stop();  // by black hole
};

class LineSystem {
  float lifetime;
  float elapsed;

 public:
  std::vector<Line> particles;

  LineSystem(float lifetime);
  void AddParticle(const Line& particle);

  void Update(float dt);
  void Draw();
  bool Alive() const {
    return elapsed <= lifetime;
  }
};

LineSystem SpawnTriangle(Triangle2D tri, float lifetime, float speed);
