# Contributing to 2DGameSFML

Thanks for taking the time to contribute. This is a small SFML game, and
straightforward pull requests are welcome.

## Ways to help

- Report bugs with steps to reproduce
- Suggest levels, controls, or UI improvements
- Fix crashes, asset-loading issues, or build problems
- Improve documentation

Please read the [Code of Conduct](CODE_OF_CONDUCT.md) before participating.

## Development setup

You need CMake 3.16+, a C++17 compiler, and SFML 2.5+.

### Linux

```bash
sudo apt-get install cmake g++ libsfml-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/2DGameSFML
```

### Windows

1. Install [CMake](https://cmake.org/download/) and Visual Studio 2022 with C++.
2. Download [SFML 2.6.2 for VS 2022 x64](https://github.com/SFML/SFML/releases/tag/2.6.2).
3. Configure and build:

```powershell
cmake -S . -B build -A x64 -DCMAKE_PREFIX_PATH="C:\path\to\SFML-2.6.2"
cmake --build build --config Debug
```

The original `Game1.0/Game.vcxproj` still exists for Visual Studio + NuGet, but
CMake is the supported build.

## Pull requests

1. Create a branch from `main`.
2. Keep the change focused. Do not mix refactors with bug fixes.
3. Describe what you changed and how you tested it.
4. Make sure CI is green. GitHub Actions builds Windows and Linux on every PR.

## Code notes

- Game assets are loaded from the executable directory.
- Map data lives in `Game1.0/map.h`.
- Keep SFML 2.x APIs. SFML 3 is a breaking change.

## Security

Do not report vulnerabilities in public issues. See [SECURITY.md](SECURITY.md).
