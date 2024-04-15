#include "oneshot.h"

OneShot::OneShot(float time) : time(time), elapsed(0.0f) {}

bool OneShot::IsPassed() {
  if (passed) {
    return false;
  }
  if (elapsed >= time) {
    passed = true;
    return true;
  }
  return false;
}

void OneShot::Update(float dt) {
  elapsed += dt;
}

void OneShot::Reset() {
  elapsed = 0.0f;
  passed = false;
}
