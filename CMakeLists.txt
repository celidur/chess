cmake_minimum_required(VERSION 3.25)
project(chess)
#set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(chess
        view/BoardBase.h
        view/Board.h
        view/Board.cpp
        logic/Bishop.h
        logic/Bishop.cpp
        logic/King.h
        logic/King.cpp
        logic/Knight.h
        logic/Knight.cpp
        logic/Pawn.h
        logic/Pawn.cpp
        logic/Queen.h
        logic/Queen.cpp
        logic/Rook.h
        logic/Rook.cpp
        logic/Piece.h
        logic/Piece.cpp
        logic/Game.h
        logic/Game.cpp
        common/struct.h
        common/function.h
        common/function.cpp)

set(SFML_INCLUDE_DIR "./SFML/include")
set(SFML_LIBRARY_DIR "./SFML/build/lib")

include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries (chess sfml-graphics sfml-window sfml-system)
