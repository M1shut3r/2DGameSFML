#pragma once

#include <filesystem>
#include <string>

namespace game {

std::filesystem::path executableDir();
std::filesystem::path assetsRoot();
std::filesystem::path assetPath(const std::string& relative);

} // namespace game
