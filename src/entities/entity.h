#pragma once

class Entity {
  virtual void Update(float dt) = 0;
  virtual void Draw() = 0;
};
