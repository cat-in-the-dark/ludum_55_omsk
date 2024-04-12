#include "cooldown.h"

Cooldown::Cooldown(float time) : time(time), elapsed(0) {}
Cooldown::Cooldown(float time, float init) : time(time), elapsed(init) {}
bool Cooldown::Invoke() {
  if (elapsed >= time) {
    elapsed = 0;
    return true;
  }
  return false;
}
void Cooldown::Update(float dt) {
  elapsed += dt;
}
void Cooldown::Reset() {
  elapsed = 0;
}
