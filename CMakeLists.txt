cmake_minimum_required(VERSION 3.25)
project(chess)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(chess
        Board.cpp
        main.cpp
        Piece.cpp
        Player.cpp Game.h Game.cpp struct.h)

set(SFML_INCLUDE_DIR "./SFML/include")
set(SFML_LIBRARY_DIR "./SFML/build/lib")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries (chess sfml-graphics sfml-window sfml-system)

