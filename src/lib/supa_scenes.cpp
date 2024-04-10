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

TextureScene::TextureScene(Texture2D tex) : tex(tex) {}
void TextureScene::Activate() {}
void TextureScene::Update() {}
void TextureScene::Draw() {
  DrawTexture(tex, 0, 0, WHITE);
}
void TextureScene::Exit() {}
