#pragma once

#include <raylib.h>

constexpr auto kCanvasWidth = 1280;
constexpr auto kCanvasHeight = 720;

// 80/45
constexpr auto kWindowWidth = kCanvasWidth;
constexpr auto kWindowHeight = kCanvasHeight;

constexpr auto kLineThickness = 3.0f;

extern bool kDebugRender;

constexpr auto kWaveLines = 16;
constexpr auto kMaxSegmentLen = 5.0f;

constexpr auto kPlayerColor = DARKGREEN;
constexpr auto kCubeColor = GOLD;
constexpr auto kEnemyColor = VIOLET;

constexpr auto kDeathTimeout = 2.0f;
constexpr auto kWinTimeout = 2.0f;
constexpr auto kSpawnDelay = 0.05f;

constexpr auto kCameraZoom = 2.0f;

extern unsigned int current_level;
