#include "core/Collision.h"

#include "core/Constants.h"

namespace game {

CollisionResult collideWithTile(Player& player, const TileMap& map, int row, int col, bool laserOn, int timeToWin,
                                int timeLimit) {
    CollisionResult result;

    if (map.neighborHas(row, col, 's') || map.neighborHas(row, col, '0')) {
        player.x += player.dirX;
        player.y += -player.dirY;

        if (player.y >= row * kTileSize + kTileSize || player.y <= row * kTileSize) {
            if (player.x <= kTileSize) {
                player.x += -player.dirX * 2.0;
                player.y += player.dirY * 2.0;
            }
            ++player.checkX;
            player.sideLock = 2;
            player.checkY = 0;
            player.bounceCode = 1;
            result.bounced = true;
        } else {
            player.x += -player.dirX * 2.0;
            player.y += player.dirY * 2.0;
            player.bounceCode = 2;
            player.checkY = 0;
            result.bounced = true;
        }
    }

    if (map.neighborHas(row, col, 'w')) {
        result.won = (timeLimit == 0 || timeToWin < timeLimit);
    }

    const bool laserHazard = map.neighborHas(row, col, 'k') && laserOn;
    const bool lava =
        map.neighborHasNoDiag(row, col, '#') || map.neighborHas(row, col, 'l') || map.neighborHas(row, col, 'L');
    const bool invertedLaser = map.neighborHas(row, col, 'K') && !laserOn;
    if (laserHazard || lava || invertedLaser) {
        result.died = true;
    }

    if (map.neighborHas(row, col, '@') && !player.inSwamp) {
        result.enteredSwamp = true;
    }

    if (map.neighborHas(row, col, 'z') && player.inSwamp) {
        result.exitedSwamp = true;
    }

    return result;
}

} // namespace game
