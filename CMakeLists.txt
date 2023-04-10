cmake_minimum_required(VERSION 3.25)
project(chess)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(chess-sfml
        Board-sfml.cpp
        main-sfml.cpp
        Piece.cpp
        Player.cpp Game.h Game.cpp struct.h BoardBase.h)

set(SFML_INCLUDE_DIR "./SFML/include")
set(SFML_LIBRARY_DIR "./SFML/build/lib")

include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries (chess-sfml sfml-graphics sfml-window sfml-system)

# Utilise moc pour générer le code des QObjects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(chess-qt
        main-qt.cpp
        Board-qt.h
        Board-qt.cpp
        Piece.cpp
        Player.cpp
        Game.h
        Game.cpp
        struct.h
        BoardBase.h
        QtGame.cpp
        QtGame.h)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)  # Mettre ici ce qui est requis de Qt (et mettre Qt5 ou Qt6 selon la version que vous avez).
target_link_libraries(chess-qt Qt6::Core Qt6::Widgets)  # Doit être après le "add_executable" et lui passer le même nom.  Mettre encore ici ce qui est requis de Qt.
