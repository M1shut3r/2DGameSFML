# 2DGameSFML

[![CI](https://github.com/M1shut3r/2DGameSFML/actions/workflows/ci.yml/badge.svg)](https://github.com/M1shut3r/2DGameSFML/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/M1shut3r/2DGameSFML?include_prereleases)](https://github.com/M1shut3r/2DGameSFML/releases/latest)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-informational)](https://github.com/M1shut3r/2DGameSFML/releases/latest)

2D-головоломка на [SFML](https://www.sfml-dev.org/): кликните, чтобы запустить мяч, отскакивайте от стен и доберитесь до цели, пока не вышло время.

Visual Studio ставить **не нужно**. Готовые файлы лежат в [Releases](https://github.com/M1shut3r/2DGameSFML/releases/latest).

![Демонстрация игры](Assets/video.gif)

## Скачать и играть

| Платформа | Файл |
| --- | --- |
| Windows 10/11 x64 | [`2DGameSFML-windows-x64.zip`](https://github.com/M1shut3r/2DGameSFML/releases/latest/download/2DGameSFML-windows-x64.zip) |
| Linux x86_64 | [`2DGameSFML-linux-x86_64.tar.gz`](https://github.com/M1shut3r/2DGameSFML/releases/latest/download/2DGameSFML-linux-x86_64.tar.gz) |
| Linux AppImage | [`2DGameSFML-linux-x86_64.AppImage`](https://github.com/M1shut3r/2DGameSFML/releases/latest/download/2DGameSFML-linux-x86_64.AppImage) |

**Windows:** распакуйте архив и запустите `2DGameSFML.exe`. DLL уже внутри.

**Linux:**

```bash
chmod +x 2DGameSFML-linux-x86_64.AppImage
./2DGameSFML-linux-x86_64.AppImage
```

или:

```bash
tar -xf 2DGameSFML-linux-x86_64.tar.gz
cd 2DGameSFML-linux-x86_64
./2DGameSFML.sh
```

Новый релиз собирается автоматически, когда в репозиторий пушится тег `v*`.

## Как играть

- ЛКМ по полю — задать направление мяча
- Серый квадрат — выход с уровня
- Лазеры и ловушки сбрасывают мяч на старт
- Болото замедляет, зона `z` возвращает скорость
- На части уровней есть лимит времени
- Иконки слева сверху: звук, следующий трек, сброс мяча

Сначала пройдите встроенные карты без подсказок. После них работает генерация следующих уровней.

## Сборка из исходников

Нужны **CMake 3.16+**, компилятор **C++17** и **SFML 2.5+**.

### Linux

```bash
sudo apt-get install cmake g++ libsfml-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/2DGameSFML
```

### Windows

1. Установите Visual Studio 2022 (C++) и CMake.
2. Скачайте [SFML 2.6.2 VC17 64-bit](https://github.com/SFML/SFML/releases/tag/2.6.2).
3. Соберите:

```powershell
cmake -S . -B build -A x64 -DCMAKE_PREFIX_PATH="C:\path\to\SFML-2.6.2"
cmake --build build --config Release
```

Проект Visual Studio `Game1.0/Game.vcxproj` оставлен для NuGet, основной способ сборки — CMake.

## Структура репозитория

```text
2DGameSFML/
├── Game1.0/                 исходники, карты и игровые ассеты
├── Assets/video.gif         превью для README
├── packaging/               desktop-файл и текст для релизных архивов
├── scripts/                 упаковка Windows zip и Linux tar.gz / AppImage
└── .github/workflows/       CI и публикация GitHub Release
```

## Contributing

Идеи и патчи приветствуются. Как собрать проект и оформить PR — в [CONTRIBUTING.md](CONTRIBUTING.md). Правила общения — в [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## License

Код распространяется по лицензии [MIT](LICENSE). Аудио и графика в `Game1.0/` могут иметь отдельные права правообладателей.

---

## English

A small 2D SFML puzzle game. Download a ready build from
[Releases](https://github.com/M1shut3r/2DGameSFML/releases/latest) — no Visual Studio install required.

- **Windows:** extract `2DGameSFML-windows-x64.zip` and run `2DGameSFML.exe`
- **Linux:** run the AppImage, or extract the `.tar.gz` and start `./2DGameSFML.sh`
- Left click aims the ball, the gray tile is the goal, top-left icons mute / skip track / reset

Build from source with CMake and SFML 2.5+. CI compiles Windows and Linux on every pull request. Push a `v*` tag to publish a GitHub Release.
