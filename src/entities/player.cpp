#include "player.h"

#include <raylib.h>

#include "const.h"
#include "lib/renderer.h"

void Player::Update() {}

void Player::Draw() {
  DrawTriangleLinesThick(GetPlayerShape(), kLineThickness, color);
}
