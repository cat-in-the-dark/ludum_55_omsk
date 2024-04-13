#include "wave.h"

#include <rlgl.h>

WaveParticle::WaveParticle(Vector2 pos, Vector2 dir, Vector2 speed) : pos(pos), dir(dir), speed(speed) {}
void WaveParticle::Update(float dt) {
  pos.x += dir.x * speed.x * dt;
  pos.y += dir.y * speed.y * dt;
}

WaveSystem::WaveSystem(float lifetime) : lifetime(lifetime) {}
bool WaveSystem::Alive() const {
  return elapsed >= 0 && elapsed < lifetime;
}
void WaveSystem::AddParticle(WaveParticle particle) {
  particles.push_back(particle);
}
void WaveSystem::Update(float dt) {
  elapsed += dt;
  if (!Alive()) {
    return;
  }

  for (auto& particle : particles) {
    particle.Update(dt);
  }

  points.clear();
  // TODO: generate spline
  for (auto& particle : particles) {
    if (particle.alive) {
      points.emplace_back(particle.pos);
    }
  }
}

// TODO: use kirills draw thicc line
void drawLineWired(std::vector<Vector2>& model, Color color) {
  rlBegin(RL_LINES);
  rlColor4ub(color.r, color.g, color.b, color.a);
  for (size_t i = 0; i < model.size() - 1; i++) {
    rlVertex2f(model[i].x, model[i].y);
    rlVertex2f(model[i + 1].x, model[i + 1].y);
  }
  auto& last = model[model.size() - 1];
  rlVertex2f(last.x, last.y);
  rlVertex2f(model[0].x, model[0].y);
  rlEnd();
}

void drawSegmentsWired(std::vector<WaveParticle>& model, Color color) {
  for (size_t i = 0; i < model.size() - 1; i++) {
    if (!model[i].alive || !model[i + 1].alive) {
      continue;
    }
    DrawLine(model[i].pos.x, model[i].pos.y, model[i + 1].pos.x, model[i + 1].pos.y, color);
  }
}

void WaveSystem::Draw() {
  if (!Alive()) {
    return;
  }

  drawSegmentsWired(particles, RED);
  // drawLineWired(points, RED);
  // for (auto& p : points) {
  //   DrawCircle(p.x, p.y, 1, RED);
  // }
}

Vector2 Lerp2D(const Vector2& v1, const Vector2& v2, float t) {
  return Vector2{Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t)};
}

inline Triangle2D Tri60() {
  return {{0, -1}, {-float(std::sqrt(3)) / 2.0f, 0.5}, {float(std::sqrt(3)) / 2.0f, 0.5}};
}

WaveSystem SpawnTriangle(Triangle2D tri, float lifetime, float speed) {
  WaveSystem ws(lifetime);

  auto triDir = Tri60();

  Vector2 speed_vec{speed, speed};

  for (int i = 0; i < n_particles; i++) {
    float t = float(i) / float(n_particles);
    auto e1 = Lerp2D(tri.p1, tri.p2, t);
    auto dir1 = Lerp2D(triDir.p1, triDir.p2, t);
    ws.AddParticle(WaveParticle(e1, dir1, speed_vec));
  }

  for (int i = 0; i < n_particles; i++) {
    float t = float(i) / float(n_particles);
    auto e2 = Lerp2D(tri.p2, tri.p3, t);
    auto dir2 = Lerp2D(triDir.p2, triDir.p3, t);
    ws.AddParticle(WaveParticle(e2, dir2, speed_vec));
  }

  for (int i = 0; i < n_particles; i++) {
    float t = float(i) / float(n_particles);
    auto e3 = Lerp2D(tri.p3, tri.p1, t);
    auto dir3 = Lerp2D(triDir.p3, triDir.p1, t);
    ws.AddParticle(WaveParticle(e3, dir3, speed_vec));
  }

  return ws;
}
