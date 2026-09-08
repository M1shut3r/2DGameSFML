cmake_minimum_required(VERSION 3.16)

project(GameSFML VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build unit tests" ON)
option(BUILD_GAME "Build the SFML game executable" ON)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(GAME_FETCH_SFML "Download SFML when it is not installed" ON)

if(MSVC)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

function(game_set_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8 /external:W0 /external:anglebrackets)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(WARNINGS_AS_ERRORS)
        if(MSVC)
            target_compile_options(${target} PRIVATE /WX)
        else()
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_library(game_core STATIC
    src/core/Player.cpp
    src/core/MapData.cpp
    src/core/Collision.cpp
    src/core/GameLogic.cpp
    src/core/Paths.cpp
)
target_include_directories(game_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(game_core PUBLIC GAME_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")
game_set_warnings(game_core)

if(WIN32)
    target_compile_definitions(game_core PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
endif()

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        SYSTEM
    )
    FetchContent_MakeAvailable(doctest)

    add_executable(game_tests tests/test_core.cpp)
    target_link_libraries(game_tests PRIVATE game_core doctest::doctest)
    game_set_warnings(game_tests)
    add_test(NAME game_tests COMMAND game_tests)
endif()

set(SFML_FOUND OFF)
find_package(SFML 2.5 COMPONENTS Graphics Audio Window System QUIET)
if(SFML_FOUND)
    set(GAME_SFML_FROM_SYSTEM ON)
else()
    find_package(SFML 2.5 COMPONENTS graphics audio window system QUIET)
    if(SFML_FOUND)
        set(GAME_SFML_FROM_SYSTEM ON)
    endif()
endif()

if(NOT SFML_FOUND AND GAME_FETCH_SFML AND BUILD_GAME)
    include(FetchContent)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)
    set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
        SYSTEM
    )
    FetchContent_MakeAvailable(sfml)
    set(SFML_FOUND ON)
    set(GAME_SFML_FROM_SYSTEM OFF)
endif()

if(BUILD_GAME AND NOT SFML_FOUND)
    message(FATAL_ERROR "SFML was not found. Install libsfml-dev or enable GAME_FETCH_SFML.")
endif()

if(SFML_FOUND AND BUILD_GAME)
    add_executable(Game
        src/app/main.cpp
        src/app/Game.cpp
        src/app/Assets.cpp
    )
    target_link_libraries(Game PRIVATE game_core)
    game_set_warnings(Game)

    if(TARGET SFML::Graphics)
        target_link_libraries(Game PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)
    else()
        target_link_libraries(Game PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
    endif()

    if(WIN32)
        target_compile_definitions(Game PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
        set_target_properties(Game PROPERTIES WIN32_EXECUTABLE TRUE)
        if(MSVC)
            target_link_options(Game PRIVATE /ENTRY:mainCRTStartup)
        endif()
    endif()

    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:Game>/assets"
        COMMENT "Copy game assets next to the executable"
    )

    if(WIN32 AND DEFINED sfml_SOURCE_DIR)
        set(_openal_src "${sfml_SOURCE_DIR}/extlibs/bin/x64/openal32.dll")
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(_openal_src "${sfml_SOURCE_DIR}/extlibs/bin/x86/openal32.dll")
        endif()
        if(EXISTS "${_openal_src}")
            add_custom_command(TARGET Game POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${_openal_src}"
                        "$<TARGET_FILE_DIR:Game>/openal32.dll"
                COMMENT "Copy OpenAL runtime next to the executable"
            )
            install(FILES "${_openal_src}" DESTINATION .)
        endif()
    endif()

    install(TARGETS Game RUNTIME DESTINATION .)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets/" DESTINATION assets)
    install(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION .)

    set(CPACK_GENERATOR "ZIP")
    set(CPACK_PACKAGE_NAME "2DGameSFML")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_FILE_NAME "2DGameSFML-windows-x64")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
    include(CPack)
endif()
