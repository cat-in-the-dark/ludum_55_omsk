#pragma once

#include <raylib.h>

#include <functional>
#include <vector>

#include "lib/scene.h"

class StressTestScene : public Scene {
 public:
  StressTestScene();
  void Activate();
  void Exit();
  void Update();
  void Draw();

 private:
  inline Color getRandomColor() {
    return {static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(0, 255)), 255};
  }
  int count_;
  unsigned int currentTest_;
  std::vector<std::function<void()>> tests_;
};
