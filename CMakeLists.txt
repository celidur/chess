cmake_minimum_required(VERSION 3.25)
project(chess)
set(CMAKE_CXX_STANDARD 20)

include_directories(.)
# Utilise moc pour générer le code des QObjects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(chess
        main.cpp
        Board.h
        Board.cpp
        Piece.cpp
        Player.cpp
        Game.h
        Game.cpp
        struct.h
        QtGame.cpp
        QtGame.h)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)  # Mettre ici ce qui est requis de Qt (et mettre Qt5 ou Qt6 selon la version que vous avez).
target_link_libraries(chess Qt6::Core Qt6::Widgets)  # Doit être après le "add_executable" et lui passer le même nom.  Mettre encore ici ce qui est requis de Qt.
