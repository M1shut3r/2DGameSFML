#pragma once

#include "core/MapData.h"
#include "core/Player.h"

namespace game {

struct CollisionResult {
    bool bounced = false;
    bool won = false;
    bool died = false;
    bool enteredSwamp = false;
    bool exitedSwamp = false;
};

CollisionResult collideWithTile(Player& player, const TileMap& map, int row, int col, bool laserOn, int timeToWin,
                                int timeLimit);

} // namespace game
