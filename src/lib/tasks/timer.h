#pragma once

// Timer checks whether time is passed or not.
class Timer {
  const float time;
  float elapsed;

 public:
  Timer(float time);

  bool IsPassed();
  void Update(float dt);
  void Reset();
  float Elapsed();
};
