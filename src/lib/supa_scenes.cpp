#include "supa_scenes.h"

#include <raylib.h>

#include "scene_manager.h"

ComboScene::ComboScene() {}
void ComboScene::Activate() {
  for (auto& scene : scenes) {
    scene->Activate();
  }
}
void ComboScene::Update() {
  for (auto& scene : scenes) {
    scene->Update();
  }
}
void ComboScene::Draw() {
  for (auto& scene : scenes) {
    scene->Draw();
  }
}
void ComboScene::Exit() {
  for (auto& scene : scenes) {
    scene->Exit();
  }
}

KeyAwaitScene::KeyAwaitScene(SceneManager* sm, int key, std::string next) : sm(sm), key(key), next(next) {}
void KeyAwaitScene::Activate() {}
void KeyAwaitScene::Update() {
  if (GetKeyPressed() == key) {
    sm->Change(next);
  }
}
void KeyAwaitScene::Draw() {}
void KeyAwaitScene::Exit() {}

TextureScene::TextureScene(Texture2D tex, int width, int height) : tex(tex), width(width), height(height) {}
void TextureScene::Activate() {}
void TextureScene::Update() {}
void TextureScene::Draw() {
  Rectangle src{0, 0, float(tex.width), float(tex.height)};
  Rectangle dst{0, 0, float(width), float(height)};
  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);
}
void TextureScene::Exit() {}

TimerScene::TimerScene(SceneManager* sm, float seconds, std::string next) : sm{sm}, timer{seconds}, next{next} {}

void TimerScene::Activate() {
  timer.Reset();
}

void TimerScene::Update() {
  timer.Update(GetFrameTime());
  if (timer.IsPassed()) {
    sm->Change(next);
  }
}

void TimerScene::Draw() {}

void TimerScene::Exit() {}

LambdaScene::LambdaScene(std::function<void()> callback) : callback(callback) {}

void LambdaScene::Activate() {}

void LambdaScene::Update() {
  callback();
}

void LambdaScene::Draw() {}

void LambdaScene::Exit() {}
