#pragma once

#include <raylib.h>

#include <vector>

#include "entities/anti_wall.h"
#include "entities/black_hole.h"
#include "entities/circle_wall.h"
#include "entities/player.h"
#include "line.h"

void CheckCollisionAntiWall(const AntiCircleWall& wall, Line& line);

void CheckCollisionBlackHole(const std::vector<BlackHole>& black_holes, Line& line);

void CheckCollisionCircleWalls(const std::vector<CircleWall>& circle_walls, Line& line);

void CheckCollisionsPlayerCircleWalls(const std::vector<CircleWall>& circle_walls, Player& player);

void CheckCollisionsPlayerAntiWall(const AntiCircleWall& anti_wall, Player& player);
