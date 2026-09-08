#include "core/Paths.h"

#include <algorithm>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace game {
namespace {

std::filesystem::path probeAssets(const std::filesystem::path& base) {
    auto candidate = base / "assets";
    if (std::filesystem::exists(candidate / "textures" / "ball11.png")) {
        return candidate;
    }
    return {};
}

} // namespace

std::filesystem::path executableDir() {
#ifdef _WIN32
    wchar_t buffer[MAX_PATH] = {0};
    const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return std::filesystem::current_path();
    }
    return std::filesystem::path(buffer).parent_path();
#else
    char buffer[4096] = {0};
    const ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length <= 0) {
        return std::filesystem::current_path();
    }
    buffer[length] = '\0';
    return std::filesystem::path(buffer).parent_path();
#endif
}

std::filesystem::path assetsRoot() {
    const std::vector<std::filesystem::path> candidates = {
        probeAssets(executableDir()),
        probeAssets(executableDir().parent_path()),
        probeAssets(std::filesystem::current_path()),
        probeAssets(std::filesystem::current_path().parent_path()),
#ifdef GAME_ASSETS_DIR
        std::filesystem::path(GAME_ASSETS_DIR),
#endif
    };

    const auto found = std::find_if(candidates.begin(), candidates.end(), [](const std::filesystem::path& candidate) {
        return !candidate.empty() && std::filesystem::exists(candidate / "textures" / "ball11.png");
    });
    if (found != candidates.end()) {
        return *found;
    }

    return std::filesystem::current_path() / "assets";
}

std::filesystem::path assetPath(const std::string& relative) {
    return assetsRoot() / relative;
}

} // namespace game
