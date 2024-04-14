#include <raylib.h>
#include <stdlib.h>

#include "const.h"
#include "lib/scene.h"
#include "lib/scene_manager.h"
#include "lib/supa_scenes.h"
#include "scenes/collisions_test_scene.h"
#include "scenes/game_scene.h"
#include "scenes/stress_test_scene.h"
#include "scenes/test_render.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else  // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static Shader bloom;

static RenderTexture2D canvas;
static Rectangle canvasRect = {0, 0, kCanvasWidth, -kCanvasHeight};
static Vector2 canvasOrigin = {0, 0};

// Это костыль чтобы выключить шейдер вне game scene
constexpr auto GAME_SCENE = "game";
constexpr auto RENDER_SCENE = "test_render";

static Rectangle get_pixel_perfect_layout(int cw, int ch) {
  float sw = GetScreenWidth();
  float sh = GetScreenHeight();

  auto scale = fmin(sw / cw, sh / ch);

  return {
      (sw - cw * scale) * 0.5f,
      (sh - ch * scale) * 0.5f,
      cw * scale,
      ch * scale,
  };
}

void update(void* arg) {
  auto sm = reinterpret_cast<SceneManager*>(arg);
  // TODO: use pattern https://gameprogrammingpatterns.com/game-loop.html
  sm->Update();
  BeginTextureMode(canvas);
  if (!sm->Draw()) {
    ClearBackground(BLACK);
    DrawText(TextFormat("SCENE '%s' NOT FOUND", sm->Current().c_str()), 0, 0, 32, RED);
  }
  EndTextureMode();

  Rectangle windowRec = get_pixel_perfect_layout(kCanvasWidth, kCanvasHeight);
  BeginDrawing();
  // FIXME: сцены должны быть рекурсивно вложенными как в html. А то шейдер не выключить
  auto isGameScene = (sm->Current() == GAME_SCENE) || (sm->Current() == RENDER_SCENE);
  if (isGameScene) {
    BeginShaderMode(bloom);
  }
  DrawTexturePro(canvas.texture, canvasRect, windowRec, canvasOrigin, 0.0f, WHITE);
  if (isGameScene) {
    EndShaderMode();
  }
  EndDrawing();
}

int main() {
  InitWindow(kWindowWidth, kWindowHeight, "WAVE");

  bloom = LoadShader(0, TextFormat("assets/shaders/glsl%i/bloom.fs", GLSL_VERSION));
  canvas = LoadRenderTexture(kCanvasWidth, kCanvasHeight);
  SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
  // ToggleFullscreen();

  SceneManager sm;

  sm.Register<ComboScene>("logo")
      ->With<TextureScene>(LoadTexture("assets/logo.png"), kCanvasWidth, kCanvasHeight)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "title");

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/main.png"), kCanvasWidth, kCanvasHeight)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("game")->With<GameScene>(&sm)->With<KeyAwaitScene>(&sm, KEY_SPACE, "test_collisions");

  sm.Register<ComboScene>("next")
      ->With<TextureScene>(LoadTexture("assets/main.png"), kCanvasWidth, kCanvasHeight)
      ->With<TimerScene>(&sm, 3.0f, "game")
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("test_collisions")
      ->With<CollisionsTestScene>()
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "stress_test");

  sm.Register<ComboScene>("stress_test")->With<StressTestScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "title");

  sm.Register<ComboScene>("gameover")
      ->With<TextureScene>(LoadTexture("assets/lose.png"), kCanvasWidth, kCanvasHeight)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "logo");

  sm.Register<ComboScene>("gamewin")
      ->With<TextureScene>(LoadTexture("assets/winsc_2.png"), kCanvasWidth, kCanvasHeight)
      ->With<TimerScene>(&sm, 3.0f, "logo")
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "logo");

  sm.Register<ComboScene>("test_render")->With<TestRenderScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Change("title");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(update, &sm, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update(&sm);
  }
#endif

  UnloadRenderTexture(canvas);
  UnloadShader(bloom);
  CloseWindow();

  return 0;
}
