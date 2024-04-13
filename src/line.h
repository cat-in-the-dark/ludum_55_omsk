#pragma once

#include <vector>

#include "lib/types.h"

class Segment {
 public:
  Vector2 p1;
  Vector2 p2;
  float elapsed = 0;
  float lifetime;
  Color color = RED;

  Segment(Vector2 p1, Vector2 p2, float lifetime);
  bool Update(float dt);  // если сегмент закончился то тут false
  void Draw();
  float Length() const;
  bool Alive() const {
    return elapsed <= lifetime;
  }
};

class Line {
  std::vector<Segment> segments;
  bool alive = true;
  void SpawnSegment();

 public:
  Vector2 dir;
  float speed;
  float elapsed = 0;
  float lifetime;
  float segment_lifetime;

  float max_segment_len = 5;

  Line(Vector2 start, Vector2 dir, float speed, float lifetime, float segment_lifetime);
  bool Update(float dt);  // если линия закончилась, то тут false
  void Draw();
  bool StoppedGenerating() const {
    return elapsed >= lifetime || speed <= 0;
  }
  bool Alive() const;

  const Vector2& Pos() const;

  void Stop();  // by black hole
};

class LineSystem {
  float lifetime;
  bool alive = true;

 public:
  std::vector<Line> particles;

  LineSystem(float lifetime);
  void AddParticle(const Line& particle);

  void Update(float dt);
  void Draw();
  bool Alive() const {
    return alive;
  }
};

LineSystem SpawnTriangle(Triangle2D tri, float lifetime, float segment_lifetime, float speed);
