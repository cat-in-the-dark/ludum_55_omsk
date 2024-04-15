#pragma once

// OneShot checks whether time is passed or not and returns true only once
class OneShot {
  const float time;
  float elapsed;
  bool passed = false;

 public:
  OneShot(float time);

  bool IsPassed();
  void Update(float dt);
  void Reset();
};
