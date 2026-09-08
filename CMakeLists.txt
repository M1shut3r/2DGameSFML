cmake_minimum_required(VERSION 3.16)

project(2DGameSFML
    VERSION 1.0.0
    DESCRIPTION "2D puzzle game built with SFML"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8 /W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

if(TARGET SFML::Graphics)
    set(SFML_LIBRARIES SFML::Graphics SFML::Window SFML::System SFML::Audio)
else()
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)
endif()

add_executable(2DGameSFML Game1.0/Source.cpp)
target_include_directories(2DGameSFML PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Game1.0")
target_link_libraries(2DGameSFML PRIVATE ${SFML_LIBRARIES})

if(WIN32)
    # Keep a console window: the original Visual Studio project used SubSystem Console.
    set_target_properties(2DGameSFML PROPERTIES WIN32_EXECUTABLE FALSE)
    if(TARGET SFML::Graphics)
        foreach(sfml_lib IN ITEMS Graphics Window System Audio)
            add_custom_command(
                TARGET 2DGameSFML
                POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "$<TARGET_FILE:SFML::${sfml_lib}>"
                    "$<TARGET_FILE_DIR:2DGameSFML>"
                VERBATIM
            )
        endforeach()
    endif()
endif()

if(UNIX AND NOT APPLE)
    set_target_properties(2DGameSFML PROPERTIES
        BUILD_RPATH_USE_ORIGIN TRUE
        INSTALL_RPATH "$ORIGIN/lib"
    )
endif()

set(GAME_ASSETS
    Game1.0/Around_The_World.ogg
    Game1.0/Hyderabad.ogg
    Game1.0/jeremy-soule-sunrise-of-flutes.ogg
    Game1.0/marshmello.ogg
    Game1.0/music2.ogg
    Game1.0/hit.ogg
    Game1.0/background.png
    Game1.0/ball11.png
    Game1.0/music_icon.png
    Game1.0/playbutton.png
    Game1.0/reset.png
    Game1.0/CyrilicOld.ttf
)

foreach(asset IN LISTS GAME_ASSETS)
    add_custom_command(
        TARGET 2DGameSFML
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${asset}"
            "$<TARGET_FILE_DIR:2DGameSFML>"
        VERBATIM
    )
endforeach()

include(GNUInstallDirs)

install(TARGETS 2DGameSFML RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${GAME_ASSETS} DESTINATION ${CMAKE_INSTALL_BINDIR})

if(UNIX AND NOT APPLE)
    install(FILES packaging/2dgame-sfml.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES Game1.0/ball11.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps RENAME 2dgame-sfml.png)
endif()
