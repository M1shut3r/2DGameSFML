# 2DGameSFML

Первая 2D-игра на [SFML](https://www.sfml-dev.org/). Есть ручные уровни и генератор карт для бесконечной игры.

![Демонстрация работы](assets/video.gif)

## Как играть

Скачайте архив **2DGameSFML-windows-x64** из GitHub Actions (вкладка Actions → последний успешный прогон → Artifacts) и распакуйте его.

Дальше достаточно запустить `Game.exe`. Visual Studio и отдельные VC++ Redistributable **не нужны**:

- игра собрана в Release;
- SFML и runtime Visual C++ вшиты статически;
- рядом лежит только `openal32.dll` (звук) и папка `assets`.

Подходит для Windows 10/11 x64. На других компьютерах ничего дополнительно ставить не требуется.

### Управление

- ЛКМ по полю — задать направление мяча
- Иконка музыки — вкл/выкл звук
- Кнопка next — следующая композиция
- Кнопка reset — вернуть мяч на старт уровня
- Esc — выход

Серый квадрат — портал на следующий уровень. На уровнях с таймером нужно успеть попасть в портал, пока он открыт.

## Сборка из исходников

Нужны CMake 3.16+ и компилятор C++17.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

На Linux поставьте `libsfml-dev`. На Windows CMake сам скачает SFML и соберёт портативный exe:

```bash
cmake --install build --config Release --prefix package
```

После установки в `package/` будут `Game.exe`, `openal32.dll` и `assets/`.

## Проверки в CI

GitHub Actions на каждый push/PR запускает:

- **clang-format** и **cppcheck**
- **clang-tidy** на игровом ядре
- юнит-тесты на Linux и Windows
- Release-сборку Windows-архива, который можно сразу запускать

Локально:

```bash
clang-format --dry-run --Werror src tests
cppcheck --std=c++17 --enable=warning,style,performance,portability --error-exitcode=1 -I src src tests
```
