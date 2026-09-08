# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-09-08

### Added

- CMake build for Windows and Linux
- GitHub Actions CI that compiles both platforms on every pull request
- Automated GitHub Releases with a Windows `.exe` zip and a Linux package
- Community files: license, contributing guide, code of conduct, security policy, issue and PR templates

### Changed

- Assets are loaded from the executable directory, so Release archives run without extra setup
- In-game Russian text is stored as UTF-8 and works on Linux
- README now points to GitHub Releases instead of a checked-in binary

### Removed

- Debug `Game.exe`, PDB/ILK files, and SFML debug DLLs from the repository
  (install Visual Studio is no longer required to play)
