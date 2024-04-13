#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>

#include "CatmullRom.h"
#include "const.h"
#include "lib/scene.h"
#include "lib/scene_manager.h"
#include "lib/supa_scenes.h"
#include "lib/tasks/timer.h"
#include "scenes/collisions_test_scene.h"
#include "scenes/stress_test_scene.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void DrawLineWired(std::vector<Vector2>& model, Color color) {
  rlBegin(RL_LINES);
  rlColor4ub(color.r, color.g, color.b, color.a);
  for (size_t i = 0; i < model.size() - 1; i++) {
    rlVertex2f(model[i].x, model[i].y);
    rlVertex2f(model[i + 1].x, model[i + 1].y);
  }
  auto& last = model[model.size() - 1];
  rlVertex2f(last.x, last.y);
  rlVertex2f(model[0].x, model[0].y);
  rlEnd();
}

class GameScene : public Scene {
  Timer timer = {2};
  std::vector<Vector2> points = {{5, 5}, {5, 64}, {32, 90}, {64, 64}, {64, 5}};
  std::unique_ptr<Curve> curve = std::make_unique<CatmullRom>();

 public:
  void Activate() {
    curve->set_steps(20);
    curve->clear();
    auto& first = *points.begin();
    auto& last = *points.rbegin();
    curve->add_way_point({last.x, last.y, 0.0});
    for (auto& p : points) {
      curve->add_way_point({p.x, p.y, 0.0});
    }
    curve->add_way_point({first.x, first.y, 0.0});
  }
  void Exit() {}
  void Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      auto pos = GetMousePosition();
      TraceLog(LOG_INFO, "BUM %f, %f", pos.x, pos.y);
    }

    timer.Update(GetFrameTime());
  }
  void Draw() {
    ClearBackground(BLACK);
    if (timer.IsPassed()) {
      DrawRectangle(40, 64, 100, 64, BLUE);
    } else {
      DrawRectangle(40, 64, 100, 64, GREEN);
    }

    DrawLineWired(points, BLUE);
    std::vector<Vector2> outPoints;
    for (int i = 0; i < curve->node_count(); ++i) {
      auto&& point = curve->node(i);
      outPoints.push_back({static_cast<float>(point.x), static_cast<float>(point.y)});
    }
    DrawLineWired(outPoints, GREEN);
  }
};

void update(void* arg) {
  auto sm = reinterpret_cast<SceneManager*>(arg);
  // TODO: use pattern https://gameprogrammingpatterns.com/game-loop.html
  sm->Update();
  BeginDrawing();
  if (!sm->Draw()) {
    ClearBackground(BLACK);
    DrawText(TextFormat("SCENE '%s' NOT FOUND", sm->Current().c_str()), 0, 0, 32, RED);
  }
  EndDrawing();
}

int main() {
  InitWindow(kWindowWidth, kWindowHeight, "WAVE");

  SceneManager sm;

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/logo.png"))
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("game")->With<GameScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "test_collisions");

  sm.Register<ComboScene>("test_collisions")
      ->With<CollisionsTestScene>()
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "stress_test");

  sm.Register<ComboScene>("stress_test")->With<StressTestScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "title");

  sm.Change("title");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(update, &sm, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update(&sm);
  }
#endif

  CloseWindow();

  return 0;
}
