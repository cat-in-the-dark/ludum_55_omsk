#pragma once

#include <raylib.h>

constexpr auto kCanvasWidth = 640;
constexpr auto kCanvasHeight = 360;

// 80/45
constexpr auto kWindowWidth = kCanvasWidth * 2;
constexpr auto kWindowHeight = kCanvasHeight * 2;

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

extern unsigned int current_level;
