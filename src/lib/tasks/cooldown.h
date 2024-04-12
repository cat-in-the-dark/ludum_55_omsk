#pragma once

// Cooldown is a Timer with auto reset after passing
class Cooldown {
  const float time;
  float elapsed;

 public:
  Cooldown(float time, float init);
  Cooldown(float time);

  bool Invoke();
  void Update(float dt);
  void Reset();
};
