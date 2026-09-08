#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace game {

class AssetError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class Assets {
public:
    Assets();

    sf::Texture ball;
    sf::Texture background;
    sf::Texture musicIcon;
    sf::Texture nextButton;
    sf::Texture resetButton;
    sf::Font font;
    sf::SoundBuffer hitBuffer;

    const std::vector<std::string>& musicTracks() const {
        return musicTracks_;
    }

private:
    std::vector<std::string> musicTracks_{};
};

} // namespace game
