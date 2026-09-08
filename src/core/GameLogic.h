#pragma once

#include "core/MapData.h"
#include "core/Player.h"

namespace game {

class GameLogic {
public:
    GameLogic();

    void resetBall();
    void startLevel(int level);
    void launchTowards(double targetX, double targetY);
    void tickLaser();
    void updatePhysics();

    const Player& player() const {
        return player_;
    }
    const TileMap& map() const {
        return map_;
    }
    int level() const {
        return level_;
    }
    int timeToWin() const {
        return timeToWin_;
    }
    int timeLimit() const {
        return timeLimit_;
    }
    bool laserOn() const {
        return laserOn_;
    }
    int laserFrame() const {
        return laserFrame_;
    }
    bool justWon() const {
        return justWon_;
    }
    bool justDied() const {
        return justDied_;
    }
    bool justBounced() const {
        return justBounced_;
    }

    static bool laserOnAtFrame(int frame);

private:
    void applySpawn();

    Player player_{};
    TileMap map_{};
    int level_ = 1;
    int timeToWin_ = 0;
    int timeLimit_ = 0;
    int laserFrame_ = 0;
    bool laserOn_ = true;
    bool justWon_ = false;
    bool justDied_ = false;
    bool justBounced_ = false;
};

} // namespace game
