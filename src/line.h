#pragma once

#include <vector>

#include "lib/types.h"

class Segment {
  bool Alive() const {
    return elapsed <= lifetime;
  }

 public:
  Vector2 p1;
  Vector2 p2;
  Color color;
  float lifetime;
  float elapsed = 0;

  Segment(Vector2 p1, Vector2 p2, Color color, float lifetime, float elapsed = 0);
  bool Update(float dt);  // если сегмент закончился то тут false
  void Draw();
  float Length() const;
};

class Line {
  std::vector<Segment> segments;
  bool alive = true;
  void SpawnSegment();

 public:
  Vector2 dir;
  Color color;  // next segment color
  float speed;
  float elapsed = 0;
  float lifetime;
  float segment_lifetime;

  Line(Vector2 start, Vector2 dir, Color color, float speed, float lifetime, float segment_lifetime);
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
  bool alive = true;

 public:
  std::vector<Line> particles;

  void AddParticle(const Line& particle);

  void Update(float dt);
  void Draw();
  bool Alive() const {
    return alive;
  }
};

LineSystem SpawnTriangle(Triangle2D tri, float lifetime, float segment_lifetime, float speed);
LineSystem SpawnCircle(const Circle& circle, float lifetime, float segment_lifetime, float speed, int n_lines);
