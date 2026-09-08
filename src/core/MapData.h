#pragma once

#include "core/Constants.h"

#include <string>
#include <vector>

namespace game {

struct SpawnPoint {
    double x = 0.0;
    double y = 0.0;
};

class TileMap {
public:
    TileMap() = default;
    explicit TileMap(std::vector<std::string> rows);

    int height() const;
    int width() const;
    char at(int row, int col) const;
    bool inBounds(int row, int col) const;
    bool allRowsValid() const;
    bool neighborHas(int row, int col, char tile) const;
    bool neighborHasNoDiag(int row, int col, char tile) const;
    bool contains(char tile) const;
    bool isSpawnWalkable(const SpawnPoint& spawn) const;

    const std::vector<std::string>& rows() const {
        return rows_;
    }

    static TileMap handmade(int level);
    static TileMap generate(unsigned seed);
    static TileMap forLevel(int level);

private:
    std::vector<std::string> rows_{};
};

SpawnPoint spawnForLevel(int level);
int timeLimitForLevel(int level);
bool isSolidTile(char tile);
bool isHazardTile(char tile, bool laserOn);
bool isWinTile(char tile);

} // namespace game
