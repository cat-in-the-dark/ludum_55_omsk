#pragma once

class Scene {
 public:
  virtual void Activate() = 0;
  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void Exit() = 0;
  virtual ~Scene();
};
