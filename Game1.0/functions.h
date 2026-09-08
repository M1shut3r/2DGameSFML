#pragma once

#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <libgen.h>
#include <limits.h>
#include <unistd.h>
#endif

// Game assets are loaded by relative path. Always run next to the executable
// so GitHub Release packages work regardless of the launch directory.
inline void setWorkingDirectoryToExecutable()
{
#ifdef _WIN32
    char path[MAX_PATH] = {0};
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return;
    }
    std::string directory(path, length);
    const std::string::size_type slash = directory.find_last_of("\\/");
    if (slash != std::string::npos) {
        SetCurrentDirectoryA(directory.substr(0, slash).c_str());
    }
#else
    char path[PATH_MAX] = {0};
    const ssize_t length = readlink("/proc/self/exe", path, PATH_MAX - 1);
    if (length <= 0) {
        return;
    }
    path[length] = '\0';
    if (char* directory = dirname(path)) {
        if (chdir(directory) != 0) {
            return;
        }
    }
#endif
}
