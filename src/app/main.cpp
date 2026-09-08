#include "app/Game.h"

#include <exception>
#include <iostream>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

void showError(const std::string& message) {
#ifdef _WIN32
    MessageBoxA(nullptr, message.c_str(), "2DGameSFML", MB_ICONERROR | MB_OK);
#else
    std::cerr << message << '\n';
#endif
}

} // namespace

int main() {
    try {
        game::Game game;
        return game.run();
    } catch (const std::exception& ex) {
        showError(std::string("Не удалось запустить игру: ") + ex.what());
        return 1;
    }
}
