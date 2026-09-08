#pragma once

namespace game {

struct Player {
    double x = 0.0;
    double y = 0.0;
    double dirX = 0.0;
    double dirY = 0.0;
    double speed = 0.0;
    int bounceCode = 0;
    int checkX = 0;
    int checkY = 0;
    int sideLock = 0;
    bool inSwamp = false;

    void setStartPosition(double startX, double startY);
    void stop();
    void launchTowards(double targetX, double targetY);
    void applyBounce();
};

} // namespace game
