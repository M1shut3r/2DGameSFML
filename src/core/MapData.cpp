#include "core/MapData.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace game {
namespace {

// clang-format off
const std::vector<std::string> kLevel1 = {
    "0000000000000000000000000000000000000000",
    "0                                      0",
    "0                               ww     0",
    "0                               ww     0",
    "0                                      0",
    "0             s#                      l0",
    "0lkkkkkkkkkkkkl#                      k0",
    "0             s#                      k0",
    "0             s#                      k0",
    "0             s#                      k0",
    "0                          s#         k0",
    "0#                         s#         k0",
    "0                          s#         k0",
    "0                          s#         k0",
    "0        s                 s#         k0",
    "0        s                            k0",
    "0        s                            k0",
    "0        s#                           k0",
    "0                             ssssssssl0",
    "0                                      0",
    "0#                                     0",
    "0#                                     0",
    "0#                                     0",
    "0#                                     0",
    "0000000000000000000000000000000000000000",
};

const std::vector<std::string> kLevel2 = {
    "0000000000000000000000000000000000000000",
    "0            sl              swwwwwwwww0",
    "0            sk   @@@@@@@@   swwwwwwwww0",
    "0            sk              swwwwwwwww0",
    "0             k              s         0",
    "0             k      s#      s         0",
    "0             k      s#                0",
    "0    ssssss   k      s                 0",
    "0             k      sLKKKKKKKKKKKKKKKL0",
    "0             k      s                 0",
    "0             k      s                 0",
    "0             k      s                 0",
    "0             k      s#                0",
    "0             k      s#     ssssssss   0",
    "0             k                        0",
    "0             k                        0",
    "0             k                        0",
    "0             k     @@@@@@@@           0",
    "0             l                        0",
    "0 @@@@ sssssssssssss                   0",
    "0          zz                          0",
    "0        zz  zz                        0",
    "0LKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKL0",
    "0LKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKL0",
    "0000000000000000000000000000000000000000",
};

const std::vector<std::string> kLevel3 = {
    "0000000000000000000000000000000000000000",
    "0wwwwwwwww                             0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0                                      0",
    "0000000000000000000000000000000000000000",
};
// clang-format on

void padRows(std::vector<std::string>& rows) {
    for (auto& row : rows) {
        if (static_cast<int>(row.size()) < kMapWidth) {
            row.append(static_cast<std::size_t>(kMapWidth) - row.size(), ' ');
        } else if (static_cast<int>(row.size()) > kMapWidth) {
            row.resize(static_cast<std::size_t>(kMapWidth));
        }
    }
    while (static_cast<int>(rows.size()) < kMapHeight) {
        rows.emplace_back(kMapWidth, '0');
    }
    if (static_cast<int>(rows.size()) > kMapHeight) {
        rows.resize(static_cast<std::size_t>(kMapHeight));
    }
}

} // namespace

TileMap::TileMap(std::vector<std::string> rows) : rows_(std::move(rows)) {
    padRows(rows_);
}

int TileMap::height() const {
    return static_cast<int>(rows_.size());
}

int TileMap::width() const {
    return rows_.empty() ? 0 : static_cast<int>(rows_.front().size());
}

bool TileMap::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < height() && col < width();
}

char TileMap::at(int row, int col) const {
    if (!inBounds(row, col)) {
        return '0';
    }
    return rows_[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}

bool TileMap::allRowsValid() const {
    if (height() != kMapHeight || width() != kMapWidth) {
        return false;
    }
    return std::all_of(rows_.begin(), rows_.end(),
                       [](const std::string& row) { return static_cast<int>(row.size()) == kMapWidth; });
}

bool TileMap::neighborHas(int row, int col, char tile) const {
    return at(row, col) == tile || at(row, col + 1) == tile || at(row + 1, col) == tile || at(row + 1, col + 1) == tile;
}

bool TileMap::neighborHasNoDiag(int row, int col, char tile) const {
    return at(row, col) == tile || at(row + 1, col) == tile;
}

bool TileMap::contains(char tile) const {
    return std::any_of(rows_.begin(), rows_.end(),
                       [tile](const std::string& row) { return row.find(tile) != std::string::npos; });
}

bool TileMap::isSpawnWalkable(const SpawnPoint& spawn) const {
    const int row = static_cast<int>(spawn.y) / kTileSize;
    const int col = static_cast<int>(spawn.x) / kTileSize;
    const char tile = at(row, col);
    return !isSolidTile(tile) && !isHazardTile(tile, true) && !isHazardTile(tile, false);
}

TileMap TileMap::handmade(int level) {
    const std::vector<std::string>* packed = nullptr;
    switch (level) {
    case 1:
        packed = &kLevel1;
        break;
    case 2:
        packed = &kLevel2;
        break;
    case 3:
        packed = &kLevel3;
        break;
    default:
        throw std::out_of_range("handmade level must be 1..3");
    }
    return TileMap(*packed);
}

TileMap TileMap::generate(unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> colDist(2, kMapWidth - 3);
    std::uniform_int_distribution<int> rowDist(2, kMapHeight - 3);
    std::uniform_int_distribution<int> kindDist(0, 6);

    std::vector<std::string> generated(static_cast<std::size_t>(kMapHeight), std::string(kMapWidth, ' '));
    for (int x = 0; x < kMapWidth; ++x) {
        generated[0][static_cast<std::size_t>(x)] = '0';
        generated[static_cast<std::size_t>(kMapHeight - 1)][static_cast<std::size_t>(x)] = '0';
    }
    for (int y = 0; y < kMapHeight; ++y) {
        generated[static_cast<std::size_t>(y)][0] = '0';
        generated[static_cast<std::size_t>(y)][static_cast<std::size_t>(kMapWidth - 1)] = '0';
    }

    const auto setGeneratedTile = [&](int tileRow, int tileCol, char tile) {
        if (tileRow > 0 && tileRow < kMapHeight - 1 && tileCol > 0 && tileCol < kMapWidth - 1) {
            generated[static_cast<std::size_t>(tileRow)][static_cast<std::size_t>(tileCol)] = tile;
        }
    };

    for (int i = 0; i < 28; ++i) {
        const int y = rowDist(rng);
        const int x = colDist(rng);
        const int kind = kindDist(rng);
        if (kind <= 2) {
            setGeneratedTile(y, x, 's');
        } else if (kind == 3) {
            setGeneratedTile(y, x, '#');
        } else if (kind == 4) {
            setGeneratedTile(y, x, 'k');
        } else if (kind == 5) {
            setGeneratedTile(y, x, '@');
        } else {
            setGeneratedTile(y, x, 'l');
        }
    }

    // Keep a safe spawn pocket in the bottom-right, matching handmade level 1.
    for (int y = kMapHeight - 5; y < kMapHeight - 1; ++y) {
        for (int x = kMapWidth - 8; x < kMapWidth - 1; ++x) {
            generated[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = ' ';
        }
    }

    for (int y = 1; y <= 3; ++y) {
        for (int x = 1; x <= 4; ++x) {
            generated[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = 'w';
        }
    }

    return TileMap(std::move(generated));
}

TileMap TileMap::forLevel(int level) {
    if (level <= kHandmadeLevelCount) {
        return handmade(level);
    }
    return generate(static_cast<unsigned>(level) * 1337u + 42u);
}

SpawnPoint spawnForLevel(int level) {
    switch (level) {
    case 1:
        return {1120.0, 704.0};
    case 2:
        return {160.0, 96.0};
    case 3:
        return {50.0, 50.0};
    default:
        return {1120.0, 704.0};
    }
}

int timeLimitForLevel(int level) {
    switch (level) {
    case 1:
        return 0;
    case 2:
        return 1250;
    case 3:
        return 500;
    default:
        return 900;
    }
}

bool isSolidTile(char tile) {
    return tile == 's' || tile == '0';
}

bool isHazardTile(char tile, bool laserOn) {
    if (tile == '#' || tile == 'l' || tile == 'L') {
        return true;
    }
    if (tile == 'k' && laserOn) {
        return true;
    }
    if (tile == 'K' && !laserOn) {
        return true;
    }
    return false;
}

bool isWinTile(char tile) {
    return tile == 'w';
}

} // namespace game
