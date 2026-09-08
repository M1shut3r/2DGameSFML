#include "app/Assets.h"

#include "core/Paths.h"

namespace game {
namespace {

void loadTexture(sf::Texture& texture, const std::string& relative) {
    const auto path = assetPath(relative);
    if (!texture.loadFromFile(path.string())) {
        throw AssetError("Failed to load texture: " + path.string());
    }
}

} // namespace

Assets::Assets() {
    loadTexture(ball, "textures/ball11.png");
    loadTexture(background, "textures/background.png");
    loadTexture(musicIcon, "textures/music_icon.png");
    loadTexture(nextButton, "textures/playbutton.png");
    loadTexture(resetButton, "textures/reset.png");
    ball.setSmooth(true);

    const auto fontPath = assetPath("fonts/CyrilicOld.ttf");
    if (!font.loadFromFile(fontPath.string())) {
        throw AssetError("Failed to load font: " + fontPath.string());
    }

    const auto hitPath = assetPath("audio/hit.ogg");
    if (!hitBuffer.loadFromFile(hitPath.string())) {
        throw AssetError("Failed to load sound: " + hitPath.string());
    }

    musicTracks_ = {
        "audio/jeremy-soule-sunrise-of-flutes.ogg",
        "audio/Around_The_World.ogg",
        "audio/Hyderabad.ogg",
        "audio/music2.ogg",
        "audio/marshmello.ogg",
    };
}

} // namespace game
