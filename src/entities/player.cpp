#include "player.h"

#include <raylib.h>

#include "const.h"
#include "lib/renderer.h"

void Player::Update(__attribute__((unused)) float dt) {}

void Player::Draw() {
  DrawTriangleLinesThick(GetPlayerShape(), kLineThickness, color);
}
