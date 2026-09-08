#include "app/Game.h"

#include "core/Constants.h"
#include "core/Paths.h"

#include <sstream>
#include <string>

namespace game {

Game::Game()
    : window_(sf::VideoMode(static_cast<unsigned>(kWindowWidth), static_cast<unsigned>(kWindowHeight)), "2DGameSFML"),
      ball_(assets_.ball), background_(assets_.background), musicIcon_(assets_.musicIcon),
      nextButton_(assets_.nextButton), resetButton_(assets_.resetButton), hint_("", assets_.font, 18),
      timer_("", assets_.font, 20), hit_(assets_.hitBuffer) {
    window_.setFramerateLimit(kTargetFps);
    window_.setVerticalSyncEnabled(true);
    window_.setKeyRepeatEnabled(false);

    musicIcon_.setPosition(64.f, 0.f);
    nextButton_.setPosition(96.f, 0.f);
    resetButton_.setPosition(128.f, 0.f);

    hint_.setStyle(sf::Text::Bold | sf::Text::Underlined);
    timer_.setStyle(sf::Text::Bold | sf::Text::Underlined);
    hit_.setVolume(30.f);

    playCurrentTrack();
}

sf::String Game::utf8(const std::string& text) const {
    return sf::String::fromUtf8(text.begin(), text.end());
}

void Game::playCurrentTrack() {
    const auto path = assetPath(assets_.musicTracks().at(static_cast<std::size_t>(musicIndex_)));
    if (!music_.openFromFile(path.string())) {
        return;
    }
    music_.setVolume(musicMuted_ ? 0.f : 50.f);
    music_.setLoop(true);
    music_.play();
}

void Game::processEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window_.close();
        }
    }
}

void Game::handleButtons() {
    if (uiDebounce_ < kUiDebounceFrames) {
        ++uiDebounce_;
        return;
    }

    const sf::Vector2i mouse = sf::Mouse::getPosition(window_);
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        return;
    }

    if (sf::IntRect(64, 0, 32, 32).contains(mouse)) {
        musicMuted_ = !musicMuted_;
        musicIcon_.setColor(musicMuted_ ? sf::Color::Red : sf::Color::White);
        music_.setVolume(musicMuted_ ? 0.f : 50.f);
        uiDebounce_ = 0;
        return;
    }

    if (sf::IntRect(96, 0, 32, 32).contains(mouse)) {
        musicIndex_ = (musicIndex_ + 1) % static_cast<int>(assets_.musicTracks().size());
        playCurrentTrack();
        uiDebounce_ = 0;
        return;
    }

    if (sf::IntRect(128, 0, 32, 32).contains(mouse)) {
        logic_.resetBall();
        uiDebounce_ = 0;
        return;
    }

    if (logic_.player().speed <= 0.0) {
        logic_.launchTowards(static_cast<double>(mouse.x), static_cast<double>(mouse.y));
        uiDebounce_ = 0;
    }
}

void Game::update() {
    logic_.tickLaser();
    handleButtons();
    logic_.updatePhysics();
    if (logic_.justBounced()) {
        hit_.play();
    }
}

void Game::render() {
    window_.clear();

    const TileMap& map = logic_.map();
    for (int row = 0; row < map.height(); ++row) {
        for (int col = 0; col < map.width(); ++col) {
            const char tile = map.at(row, col);
            sf::IntRect rect(0, 0, kTileSize, kTileSize);
            switch (tile) {
            case 's':
                rect.left = 32;
                break;
            case '0':
                rect.left = 64;
                break;
            case 'w':
                if (logic_.timeLimit() == 0 || logic_.timeToWin() < logic_.timeLimit()) {
                    rect.left = 96;
                }
                break;
            case 'l':
                rect.left = 128;
                break;
            case 'k':
                rect.left = logic_.laserOn() ? 160 : 0;
                break;
            case '#':
                rect.left = 192;
                break;
            case '@':
                rect.left = 224;
                break;
            case 'L':
                rect.left = 256;
                break;
            case 'K':
                rect.left = logic_.laserOn() ? 0 : 288;
                break;
            case 'z':
                rect.left = 320;
                break;
            default:
                break;
            }
            background_.setTextureRect(rect);
            background_.setPosition(static_cast<float>(col * kTileSize), static_cast<float>(row * kTileSize));
            window_.draw(background_);
        }
    }

    if (logic_.level() == 1) {
        hint_.setString(utf8("Это обучающий уровень. Попади мячом в серый квадрат, чтобы перейти дальше.\n"
                             "Подсказка: кликни по экрану, чтобы задать направление."));
        hint_.setPosition(200.f, 0.f);
        timer_.setString("");
    } else {
        std::ostringstream timeNow;
        std::ostringstream timeLimit;
        timeNow << logic_.timeToWin();
        timeLimit << logic_.timeLimit();
        hint_.setString(utf8("Таймер: " + timeNow.str() + "   Уровень: " + std::to_string(logic_.level())));
        timer_.setString(utf8("Время на прохождение: " + timeLimit.str()));
        hint_.setPosition(400.f, 0.f);
        timer_.setPosition(700.f, 0.f);
    }

    ball_.setPosition(static_cast<float>(logic_.player().x), static_cast<float>(logic_.player().y));
    window_.draw(musicIcon_);
    window_.draw(nextButton_);
    window_.draw(resetButton_);
    window_.draw(hint_);
    window_.draw(timer_);
    window_.draw(ball_);
    window_.display();
}

int Game::run() {
    while (window_.isOpen()) {
        processEvents();
        update();
        render();
    }
    return 0;
}

} // namespace game
