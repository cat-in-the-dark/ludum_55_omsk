#include "stress_test_scene.h"

#include <raylib.h>

#include <algorithm>

#include "const.h"
#include "lib/renderer.h"

StressTestScene::StressTestScene() : count_{100}, currentTest_{0} {
  tests_ = {[this]() {
              for (auto i = 0; i < count_; i++) {
                auto color = getRandomColor();
                DrawCircleLines(GetRandomValue(0, kCanvasWidth), GetRandomValue(0, kCanvasHeight), 10.0f, color);
              }
            },
            [this]() {
              for (auto i = 0; i < count_; i++) {
                auto color = getRandomColor();
                DrawLineThick(GetRandomValue(0, kCanvasWidth), GetRandomValue(0, kCanvasHeight),
                              GetRandomValue(0, kCanvasWidth), GetRandomValue(0, kCanvasHeight), 5.0f, color);
              }
            }};
}

void StressTestScene::Activate() {
  count_ = 100;
}

void StressTestScene::Exit() {}

void StressTestScene::Update() {
  if (IsKeyPressed(KEY_ENTER)) {
    currentTest_++;
    if (currentTest_ >= tests_.size()) {
      currentTest_ = 0;
    }
  }
  if (IsKeyDown(KEY_UP)) {
    count_++;
  } else if (IsKeyDown(KEY_PAGE_UP)) {
    count_ += 10;
  } else if (count_ > 0) {
    if (IsKeyDown(KEY_DOWN)) {
      count_--;
    } else if (IsKeyDown(KEY_PAGE_DOWN)) {
      count_ -= std::min(10, count_);
    }
  }
}

void StressTestScene::Draw() {
  ClearBackground(BLACK);
  tests_[currentTest_]();

  auto text = TextFormat(
      "%05d Press up/down pgUp/pgDn\n"
      "to change objects count,\n"
      "enter to switch test",
      count_);
  DrawText(text, 7, 32, 20, BLACK);
  DrawText(text, 5, 30, 20, LIME);
  DrawFPS(5, 5);
}
