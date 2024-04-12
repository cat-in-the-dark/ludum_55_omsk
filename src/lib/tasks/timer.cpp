#include "timer.h"

Timer::Timer(float time) : time(time) {}
bool Timer::IsPassed() {
  return elapsed >= time;
}
void Timer::Update(float dt) {
  elapsed += dt;
}
void Timer::Reset() {
  elapsed = 0;
}
