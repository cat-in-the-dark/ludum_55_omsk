#pragma once

#include <raylib.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "lib/tasks/timer.h"
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

class TimerScene : public Scene {
  SceneManager* sm;
  Timer timer;
  std::string next;

 public:
  TimerScene(SceneManager* sm, float seconds, std::string next);
  void Activate();
  void Update();
  void Draw();
  void Exit();
};

class LambdaScene : public Scene {
  std::function<void()> callback;

 public:
  LambdaScene(std::function<void()> callback);
  void Activate();
  void Update();
  void Draw();
  void Exit();
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
  int width;
  int height;

 public:
  TextureScene(Texture2D tex, int width, int height);
  void Activate();
  void Update();
  void Draw();
  void Exit();
};
