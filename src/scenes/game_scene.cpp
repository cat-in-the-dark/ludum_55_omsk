#include "game_scene.h"

#include "lib/renderer.h"

void GameScene::Activate() {
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

void GameScene::Exit() {}

void GameScene::Update() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    auto pos = GetMousePosition();
    TraceLog(LOG_INFO, "BUM %f, %f", pos.x, pos.y);
  }

  timer.Update(GetFrameTime());
}

void GameScene::Draw() {
  ClearBackground(BLACK);
  if (timer.IsPassed()) {
    DrawRectangle(40, 64, 100, 64, BLUE);
  } else {
    DrawRectangle(40, 64, 100, 64, GREEN);
  }

  DrawLines(points, 5.0f, BLUE);
  std::vector<Vector2> outPoints;
  for (int i = 0; i < curve->node_count(); ++i) {
    auto&& point = curve->node(i);
    outPoints.push_back({static_cast<float>(point.x), static_cast<float>(point.y)});
  }
  DrawLines(outPoints, 5.0f, GREEN);
}
