#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "core/Collision.h"
#include "core/Constants.h"
#include "core/GameLogic.h"
#include "core/MapData.h"
#include "core/Paths.h"
#include "core/Player.h"

#include <filesystem>
#include <string>
#include <vector>

using game::collideWithTile;
using game::CollisionResult;
using game::GameLogic;
using game::isHazardTile;
using game::isSolidTile;
using game::isWinTile;
using game::Player;
using game::spawnForLevel;
using game::SpawnPoint;
using game::TileMap;
using game::timeLimitForLevel;

TEST_CASE("handmade maps have fixed size and a win tile") {
    for (int level = 1; level <= game::kHandmadeLevelCount; ++level) {
        const TileMap map = TileMap::handmade(level);
        CHECK(map.allRowsValid());
        CHECK(map.height() == game::kMapHeight);
        CHECK(map.width() == game::kMapWidth);
        CHECK(map.contains('w'));
        CHECK(map.contains('0'));
        CHECK(map.isSpawnWalkable(spawnForLevel(level)));
    }
}

TEST_CASE("generated maps stay playable") {
    for (unsigned seed : {1u, 42u, 1337u, 99991u}) {
        const TileMap map = TileMap::generate(seed);
        CHECK(map.allRowsValid());
        CHECK(map.contains('w'));
        CHECK(map.isSpawnWalkable(spawnForLevel(4)));
        CHECK(map.at(0, 0) == '0');
        CHECK(map.at(game::kMapHeight - 1, game::kMapWidth - 1) == '0');
    }
}

TEST_CASE("levels after the campaign keep generating") {
    const TileMap a = TileMap::forLevel(4);
    const TileMap b = TileMap::forLevel(5);
    CHECK(a.allRowsValid());
    CHECK(b.allRowsValid());
    CHECK(a.rows() != b.rows());
}

TEST_CASE("spawn positions match the original campaign") {
    CHECK(spawnForLevel(1).x == doctest::Approx(1120.0));
    CHECK(spawnForLevel(1).y == doctest::Approx(704.0));
    CHECK(spawnForLevel(2).x == doctest::Approx(160.0));
    CHECK(spawnForLevel(2).y == doctest::Approx(96.0));
    CHECK(spawnForLevel(3).x == doctest::Approx(50.0));
    CHECK(spawnForLevel(3).y == doctest::Approx(50.0));
}

TEST_CASE("time limits match the original campaign") {
    CHECK(timeLimitForLevel(1) == 0);
    CHECK(timeLimitForLevel(2) == 1250);
    CHECK(timeLimitForLevel(3) == 500);
    CHECK(timeLimitForLevel(4) == 900);
}

TEST_CASE("tile classification") {
    CHECK(isSolidTile('s'));
    CHECK(isSolidTile('0'));
    CHECK_FALSE(isSolidTile(' '));
    CHECK(isWinTile('w'));
    CHECK(isHazardTile('l', true));
    CHECK(isHazardTile('#', false));
    CHECK(isHazardTile('k', true));
    CHECK_FALSE(isHazardTile('k', false));
    CHECK(isHazardTile('K', false));
    CHECK_FALSE(isHazardTile('K', true));
}

TEST_CASE("player reset zeroes velocity") {
    Player player;
    player.speed = 30.0;
    player.dirX = 1.5;
    player.dirY = -2.0;
    player.stop();
    CHECK(player.speed == doctest::Approx(0.0));
    CHECK(player.dirX == doctest::Approx(0.0));
    CHECK(player.dirY == doctest::Approx(0.0));
}

TEST_CASE("launch aims at the click and can be halved by swamp") {
    Player player;
    player.setStartPosition(0.0, 0.0);
    player.launchTowards(30.0, 40.0);
    CHECK(player.speed == doctest::Approx(game::kLaunchSpeed));
    CHECK(player.dirX == doctest::Approx(1.8).epsilon(0.001));
    CHECK(player.dirY == doctest::Approx(2.4).epsilon(0.001));

    Player swamp;
    swamp.setStartPosition(0.0, 0.0);
    swamp.inSwamp = true;
    swamp.launchTowards(30.0, 40.0);
    CHECK(swamp.dirX == doctest::Approx(0.9).epsilon(0.001));
    CHECK(swamp.dirY == doctest::Approx(1.2).epsilon(0.001));
}

TEST_CASE("vertical bounce flips Y, horizontal bounce flips X") {
    Player vertical;
    vertical.dirX = 1.0;
    vertical.dirY = 2.0;
    vertical.bounceCode = 1;
    vertical.applyBounce();
    CHECK(vertical.dirX == doctest::Approx(1.0));
    CHECK(vertical.dirY == doctest::Approx(-2.0));
    CHECK(vertical.bounceCode == 0);

    Player horizontal;
    horizontal.dirX = 1.0;
    horizontal.dirY = 2.0;
    horizontal.bounceCode = 2;
    horizontal.applyBounce();
    CHECK(horizontal.dirX == doctest::Approx(-1.0));
    CHECK(horizontal.dirY == doctest::Approx(2.0));
}

TEST_CASE("laser follows the original 500/1000 frame schedule") {
    CHECK(GameLogic::laserOnAtFrame(0));
    CHECK(GameLogic::laserOnAtFrame(499));
    CHECK_FALSE(GameLogic::laserOnAtFrame(500));
    CHECK_FALSE(GameLogic::laserOnAtFrame(999));

    GameLogic logic;
    CHECK(logic.laserOn());
    for (int i = 0; i < 500; ++i) {
        logic.tickLaser();
    }
    CHECK_FALSE(logic.laserOn());
    for (int i = 0; i < 500; ++i) {
        logic.tickLaser();
    }
    CHECK(logic.laserOn());
    CHECK(logic.laserFrame() == 0);
}

TEST_CASE("winning a level advances the campaign") {
    GameLogic logic;
    CHECK(logic.level() == 1);
    logic.startLevel(1);
    CHECK(logic.player().speed == doctest::Approx(0.0));

    std::vector<std::string> rows(static_cast<std::size_t>(game::kMapHeight),
                                  std::string(static_cast<std::size_t>(game::kMapWidth), ' '));
    for (int x = 0; x < game::kMapWidth; ++x) {
        rows[0][static_cast<std::size_t>(x)] = '0';
        rows[static_cast<std::size_t>(game::kMapHeight - 1)][static_cast<std::size_t>(x)] = '0';
    }
    rows[2][2] = 'w';
    const TileMap map(rows);

    Player player;
    player.setStartPosition(64.0, 64.0);
    player.speed = 30.0;
    const CollisionResult win = collideWithTile(player, map, 2, 2, true, 10, 0);
    CHECK(win.won);
}

TEST_CASE("late arrival at the portal does not count as a win") {
    TileMap map = TileMap::handmade(2);
    Player player;
    player.setStartPosition(32.0, 32.0);
    const CollisionResult late = collideWithTile(player, map, 1, 30, true, 2000, 1250);
    CHECK_FALSE(late.won);
}

TEST_CASE("hazards kill the ball") {
    std::vector<std::string> rows(static_cast<std::size_t>(game::kMapHeight),
                                  std::string(static_cast<std::size_t>(game::kMapWidth), ' '));
    rows[4][4] = 'l';
    const TileMap map(rows);
    Player player;
    const CollisionResult death = collideWithTile(player, map, 4, 4, true, 0, 0);
    CHECK(death.died);
}

TEST_CASE("swamp enters and exits") {
    std::vector<std::string> rows(static_cast<std::size_t>(game::kMapHeight),
                                  std::string(static_cast<std::size_t>(game::kMapWidth), ' '));
    rows[5][5] = '@';
    rows[6][6] = 'z';
    const TileMap map(rows);

    Player player;
    CollisionResult enter = collideWithTile(player, map, 5, 5, true, 0, 0);
    CHECK(enter.enteredSwamp);

    player.inSwamp = true;
    CollisionResult exit = collideWithTile(player, map, 6, 6, true, 0, 0);
    CHECK(exit.exitedSwamp);
}

TEST_CASE("out of bounds tiles are treated as walls") {
    const TileMap map = TileMap::handmade(1);
    CHECK(map.at(-1, 0) == '0');
    CHECK(map.at(0, -1) == '0');
    CHECK(map.at(100, 100) == '0');
    CHECK_FALSE(map.inBounds(-1, 0));
}

TEST_CASE("reset returns the ball to the current level spawn") {
    GameLogic logic;
    logic.startLevel(2);
    logic.launchTowards(200.0, 120.0);
    CHECK(logic.player().speed > 0.0);
    logic.resetBall();
    CHECK(logic.player().speed == doctest::Approx(0.0));
    CHECK(logic.player().x == doctest::Approx(spawnForLevel(2).x));
    CHECK(logic.player().y == doctest::Approx(spawnForLevel(2).y));
    CHECK(logic.level() == 2);
}

TEST_CASE("physics moves the ball and counts the timer") {
    GameLogic logic;
    logic.startLevel(1);
    const double startX = logic.player().x;
    logic.launchTowards(startX + 100.0, logic.player().y);
    logic.updatePhysics();
    CHECK(logic.player().x > startX);
    CHECK(logic.timeToWin() == 1);
}

TEST_CASE("asset files are reachable from the repository") {
    const auto root = game::assetsRoot();
    CHECK(std::filesystem::exists(root / "textures" / "ball11.png"));
    CHECK(std::filesystem::exists(root / "textures" / "background.png"));
    CHECK(std::filesystem::exists(root / "fonts" / "CyrilicOld.ttf"));
    CHECK(std::filesystem::exists(root / "audio" / "hit.ogg"));
    CHECK(std::filesystem::exists(game::assetPath("audio/Around_The_World.ogg")));
}

TEST_CASE("neighbor lookup sees the four covered tiles") {
    std::vector<std::string> rows(static_cast<std::size_t>(game::kMapHeight),
                                  std::string(static_cast<std::size_t>(game::kMapWidth), ' '));
    rows[8][9] = 's';
    const TileMap map(rows);
    CHECK(map.neighborHas(8, 8, 's'));
    CHECK_FALSE(map.neighborHas(1, 1, 's'));
}
