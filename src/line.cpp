#include "line.h"

#include "const.h"
#include "lib/math.h"
#include "lib/renderer.h"

Segment::Segment(Vector2 p1, Vector2 p2, float lifetime) : p1(p1), p2(p2), lifetime(lifetime) {}
float Segment::Length() const {
  return Vector2Distance(p1, p2);
}

Line::Line(Vector2 start, Vector2 dir, float speed, float lifetime, float segment_lifetime)
    : dir(dir), speed(speed), lifetime(lifetime), segment_lifetime(segment_lifetime) {
  segments.emplace_back(start, start, segment_lifetime);
}

void Line::Draw() {
  if (!Alive()) {
    return;
  }
  for (const auto& segment : segments) {
    DrawLineThick(segment.p1, segment.p2, 2, segment.color);
  }
}

void Line::Update(float dt) {
  if (!Alive()) {
    return;
  }

  elapsed += dt;

  auto& last = segments[segments.size() - 1];
  last.p2.x += dir.x * speed;
  last.p2.y += dir.y * speed;

  if (last.Length() > max_segment_len) {
    SpawnSegment();
  }
}

void Line::SpawnSegment() {
  const auto& last = segments[segments.size() - 1];
  segments.emplace_back(last.p2, last.p2, segment_lifetime);
}

const Vector2& Line::Pos() const {
  auto& last_segment = segments[segments.size() - 1];
  return last_segment.p2;
}

void Line::Stop() {
  speed = 0;
}

LineSystem::LineSystem(float lifetime) {}
void LineSystem::AddParticle(const Line& particle) {
  particles.emplace_back(particle);
}
void LineSystem::Update(float dt) {
  if (!Alive()) {
    return;
  }

  for (auto& line : particles) {
    line.Update(dt);
  }
}
void LineSystem::Draw() {
  if (!Alive()) {
    return;
  }

  for (auto& line : particles) {
    line.Draw();
  }
}

LineSystem SpawnLines(Triangle2D tri, float lifetime, float speed) {
  LineSystem ls(lifetime);

  auto triDir = Tri60();

  const float segment_lifetime = 2;

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e1 = Lerp2D(tri.p1, tri.p2, t);
    auto dir1 = Lerp2D(triDir.p1, triDir.p2, t);
    ls.AddParticle(Line(e1, dir1, speed, lifetime, segment_lifetime));
  }

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e2 = Lerp2D(tri.p2, tri.p3, t);
    auto dir2 = Lerp2D(triDir.p2, triDir.p3, t);
    ls.AddParticle(Line(e2, dir2, speed, lifetime, segment_lifetime));
  }

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e3 = Lerp2D(tri.p3, tri.p1, t);
    auto dir3 = Lerp2D(triDir.p3, triDir.p1, t);
    ls.AddParticle(Line(e3, dir3, speed, lifetime, segment_lifetime));
  }

  return ls;
}
