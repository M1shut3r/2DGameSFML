#include "core/Player.h"

#include "core/Constants.h"

#include <cmath>

namespace game {

void Player::setStartPosition(double startX, double startY) {
    x = startX;
    y = startY;
}

void Player::stop() {
    speed = 0.0;
    dirX = 0.0;
    dirY = 0.0;
}

void Player::launchTowards(double targetX, double targetY) {
    if (speed > 0.0) {
        return;
    }

    const double dx = targetX - x;
    const double dy = targetY - y;
    const double distance = std::hypot(dx, dy);
    if (distance < 1e-6) {
        return;
    }

    speed = kLaunchSpeed;
    dirX += kLaunchFactor * speed * dx / distance;
    dirY += kLaunchFactor * speed * dy / distance;
    if (inSwamp) {
        dirX /= 2.0;
        dirY /= 2.0;
    }
}

void Player::applyBounce() {
    if (bounceCode == 1) {
        dirY = -dirY;
        ++checkX;
        bounceCode = 0;
    } else if (bounceCode == 2) {
        dirX = -dirX;
        if (checkX >= 2) {
            dirY = -dirY;
        }
        bounceCode = 0;
    }

    ++checkY;
    if (checkY == 3) {
        sideLock = 0;
        checkY = 0;
        checkX = 0;
    }
}

} // namespace game
