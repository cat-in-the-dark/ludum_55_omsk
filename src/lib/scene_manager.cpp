#include "scene_manager.h"

#include <iostream>

#include "scene.h"

SceneManager::SceneManager() {}

void SceneManager::Change(const std::string& name) {
  next = name;
}

bool SceneManager::Update() {
  if (current != next) {
    std::cout << "[INFO] change scene from '" << current << "' to '" << next << "'\n";

    if (scenes.count(current) != 0) {
      scenes[current]->Exit();
    }

    if (scenes.count(next) != 0) {
      scenes[next]->Activate();
    }

    current = next;
  }

  if (scenes.count(current) != 0) {
    scenes.at(current)->Update();
    return true;
  }
  return false;
}

bool SceneManager::Draw() {
  if (scenes.count(current) != 0) {
    scenes.at(current)->Draw();
    return true;
  }
  return false;
}

std::string SceneManager::Current() const {
  return current;
}
