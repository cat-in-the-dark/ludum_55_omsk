#include <raylib.h>
#include <stdlib.h>

#include <cmath>
#include <memory>
#include <vector>

#include "const.h"
#include "lib/scene.h"
#include "lib/scene_manager.h"
#include "lib/supa_scenes.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

class GameScene : public Scene {
 public:
  void Activate() {}
  void Exit() {}
  void Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      auto pos = GetMousePosition();
      TraceLog(LOG_INFO, "BUM %f, %f", pos.x, pos.y);
    }

    DrawRectangle(40, 64, 100, 64, BLUE);
  }
  void Draw() {
    ClearBackground(BLACK);
    DrawFPS(5, 5);
  }
};

class TitleScene : public Scene {
  SceneManager* sm;

 public:
  TitleScene(SceneManager* sm) : sm(sm) {}
  void Activate() {}
  void Exit() {}
  void Update() {
    if (IsKeyPressed(KEY_SPACE)) {
      sm->Change("game");
    }
  }
  void Draw() {
    ClearBackground(YELLOW);
  }
};

int main() {
  InitWindow(kWindowWidth, kWindowHeight, "WAVE");

  SceneManager sm;

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/logo.png"))
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("game")->With<GameScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "title");

  sm.Change("title");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(update, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    // TODO: use pattern https://gameprogrammingpatterns.com/game-loop.html
    sm.Update();
    BeginDrawing();
    if (!sm.Draw()) {
      ClearBackground(BLACK);
      DrawText(TextFormat("SCENE '%s' NOT FOUND", sm.Current().c_str()), 0, 0, 32, RED);
    }
    EndDrawing();
  }
#endif

  CloseWindow();

  return 0;
}
