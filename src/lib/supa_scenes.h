#pragma once

#include <raylib.h>

#include <memory>
#include <string>
#include <vector>

#include "scene.h"

class SceneManager;

class ComboScene : public Scene {
  std::vector<std::unique_ptr<Scene>> scenes;

 public:
  ComboScene();
  void Activate();
  void Update();
  void Draw();
  void Exit();

  template <class TScene, class... TParam>
  ComboScene* With(TParam&&... params) {
    scenes.emplace_back(std::make_unique<TScene>(std::forward<TParam>(params)...));
    return this;
  }
};

class KeyAwaitScene : public Scene {
  SceneManager* sm;
  int key;
  std::string next;

 public:
  KeyAwaitScene(SceneManager* sm, int key, std::string next);
  void Activate();
  void Update();
  void Draw();
  void Exit();
};

class TextureScene : public Scene {
  Texture2D tex;

 public:
  TextureScene(Texture2D tex);
  void Activate();
  void Update();
  void Draw();
  void Exit();
};
