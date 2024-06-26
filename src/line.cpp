#include "line.h"

#include "const.h"
#include "lib/math.h"
#include "lib/renderer.h"

Segment::Segment(Vector2 p1, Vector2 p2, Color color, float lifetime, float elapsed)
    : p1(p1), p2(p2), color(color), lifetime(lifetime), elapsed(elapsed) {}
float Segment::Length() const {
  return Vector2Distance(p1, p2);
}
bool Segment::Update(float dt) {
  if (!Alive()) {
    return false;
  }
  elapsed += dt;
  return true;
}
void Segment::Draw() {
  if (!Alive()) {
    return;
  }
  Color c = color;
  c.a = 255 * (lifetime - elapsed) / lifetime;
  DrawLineThick(p1, p2, 2, c);
}

Line::Line(Vector2 start, Vector2 dir, Color color, float speed, float lifetime, float segment_lifetime)
    : dir(dir), color(color), speed(speed), lifetime(lifetime), segment_lifetime(segment_lifetime) {
  segments.emplace_back(start, start, color, segment_lifetime);
}

void Line::Draw() {
  for (auto& segment : segments) {
    segment.Draw();
  }
}

bool Line::Update(float dt) {
  alive = false;
  for (auto& segment : segments) {
    // если все сегменты закончились, то и линия закончилась
    alive = segment.Update(dt) || alive;
  }

  elapsed += dt;
  if (StoppedGenerating()) {
    return alive;
  }

  auto& last = segments[segments.size() - 1];
  last.p2.x += dir.x * speed;
  last.p2.y += dir.y * speed;

  if (last.Length() > kMaxSegmentLen) {
    SpawnSegment();
  }

  return alive;
}

void Line::SpawnSegment() {
  const auto& last = segments[segments.size() - 1];

  auto progress = elapsed / lifetime;
  auto segment_elapsed_already = progress * progress * segment_lifetime;
  segments.emplace_back(last.p2, last.p2, color, segment_lifetime, segment_elapsed_already);
}

const Vector2& Line::Pos() const {
  auto& last_segment = segments[segments.size() - 1];
  return last_segment.p2;
}

void Line::Stop() {
  speed = 0;
}

void LineSystem::AddParticle(const Line& particle) {
  particles.emplace_back(particle);
}
void LineSystem::Update(float dt) {
  alive = false;
  for (auto& line : particles) {
    alive = line.Update(dt) || alive;
  }
}
void LineSystem::Draw() {
  for (auto& line : particles) {
    line.Draw();
  }
}

LineSystem SpawnTriangle(Triangle2D tri, float lifetime, float segment_lifetime, float speed) {
  LineSystem ls;

  auto triDir = Tri60();
  float offset = 4;

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e1 = Lerp2D(tri.p1, tri.p2, t);
    auto dir1 = Lerp2D(triDir.p1, triDir.p2, t);
    ls.AddParticle(Line(e1 + dir1 * offset, dir1, kPlayerColor, speed, lifetime, segment_lifetime));
  }

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e2 = Lerp2D(tri.p2, tri.p3, t);
    auto dir2 = Lerp2D(triDir.p2, triDir.p3, t);
    ls.AddParticle(Line(e2 + dir2 * offset, dir2, kPlayerColor, speed, lifetime, segment_lifetime));
  }

  for (int i = 0; i < kWaveLines; i++) {
    float t = float(i) / float(kWaveLines);
    auto e3 = Lerp2D(tri.p3, tri.p1, t);
    auto dir3 = Lerp2D(triDir.p3, triDir.p1, t);
    ls.AddParticle(Line(e3 + dir3 * offset, dir3, kPlayerColor, speed, lifetime, segment_lifetime));
  }

  return ls;
}

LineSystem SpawnCircle(const Circle& circle, float lifetime, float segment_lifetime, float speed, int n_lines) {
  LineSystem ls;

  for (int i = 0; i < n_lines; i++) {
    float angle = i * (2.0 * PI) / float(n_lines);
    Vector2 dir = {cosf(angle), sinf(angle)};
    Vector2 p = circle.center + dir * circle.radius;
    ls.AddParticle(Line(p + dir, dir, kEnemyColor, speed, lifetime, segment_lifetime));
  }
  return ls;
}
