#include "core/GameLogic.h"

#include "core/Collision.h"
#include "core/Constants.h"

namespace game {

GameLogic::GameLogic() {
    startLevel(1);
}

bool GameLogic::laserOnAtFrame(int frame) {
    if (frame == 0) {
        return true;
    }
    const int phase = frame % kLaserPeriodFrames;
    if (phase == 0) {
        return true;
    }
    return phase < kLaserOffFrame;
}

void GameLogic::applySpawn() {
    const SpawnPoint spawn = spawnForLevel(level_);
    player_.setStartPosition(spawn.x, spawn.y);
    player_.stop();
    player_.inSwamp = false;
    player_.bounceCode = 0;
    player_.checkX = 0;
    player_.checkY = 0;
    player_.sideLock = 0;
    timeToWin_ = 0;
}

void GameLogic::startLevel(int level) {
    level_ = level < 1 ? 1 : level;
    map_ = TileMap::forLevel(level_);
    timeLimit_ = timeLimitForLevel(level_);
    applySpawn();
}

void GameLogic::resetBall() {
    applySpawn();
}

void GameLogic::launchTowards(double targetX, double targetY) {
    player_.launchTowards(targetX, targetY);
}

void GameLogic::tickLaser() {
    ++laserFrame_;
    if (laserFrame_ == kLaserOffFrame) {
        laserOn_ = false;
    } else if (laserFrame_ == kLaserPeriodFrames) {
        laserOn_ = true;
        laserFrame_ = 0;
    }
}

void GameLogic::updatePhysics() {
    justWon_ = false;
    justDied_ = false;
    justBounced_ = false;

    if (player_.speed <= 0.0) {
        return;
    }

    if (player_.bounceCode == 0) {
        player_.x += player_.dirX;
        player_.y += player_.dirY;
        ++timeToWin_;
    }

    const int startRow = static_cast<int>(player_.y) / kTileSize;
    const int endRow = static_cast<int>(player_.y + kTileSize) / kTileSize;
    const int startCol = static_cast<int>(player_.x) / kTileSize;
    const int endCol = static_cast<int>(player_.x + kTileSize) / kTileSize;

    for (int row = startRow; row < endRow; ++row) {
        for (int col = startCol; col < endCol; ++col) {
            const CollisionResult hit = collideWithTile(player_, map_, row, col, laserOn_, timeToWin_, timeLimit_);
            if (hit.bounced) {
                justBounced_ = true;
            }
            if (hit.enteredSwamp) {
                player_.dirX /= 2.0;
                player_.dirY /= 2.0;
                player_.inSwamp = true;
            }
            if (hit.exitedSwamp) {
                player_.dirX *= 2.0;
                player_.dirY *= 2.0;
                player_.inSwamp = false;
            }
            if (hit.died) {
                justDied_ = true;
                applySpawn();
                return;
            }
            if (hit.won) {
                justWon_ = true;
                startLevel(level_ + 1);
                return;
            }
        }
    }

    player_.applyBounce();
}

} // namespace game
