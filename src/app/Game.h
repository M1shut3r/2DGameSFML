#pragma once

#include "app/Assets.h"
#include "core/GameLogic.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace game {

class Game {
public:
    Game();
    int run();

private:
    void processEvents();
    void update();
    void render();
    void handleButtons();
    void playCurrentTrack();
    sf::String utf8(const std::string& text) const;

    Assets assets_;
    GameLogic logic_;
    sf::RenderWindow window_;
    sf::Sprite ball_;
    sf::Sprite background_;
    sf::Sprite musicIcon_;
    sf::Sprite nextButton_;
    sf::Sprite resetButton_;
    sf::Text hint_;
    sf::Text timer_;
    sf::Music music_;
    sf::Sound hit_;
    int uiDebounce_ = 0;
    int musicIndex_ = 0;
    bool musicMuted_ = false;
};

} // namespace game
