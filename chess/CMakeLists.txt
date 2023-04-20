cmake_minimum_required(VERSION 3.25)
project(chess)
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
set(CMAKE_CXX_STANDARD 20)


include_directories(.)
# Utilise moc pour générer le code des QObjects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
        main.cpp
        view/Board.h
        view/Board.cpp
        logic/Piece.cpp
        logic/Player.cpp
        logic/Game.h
        logic/Game.cpp
        common/struct.h
        controller/QtGame.cpp
        controller/QtGame.h
        logic/King.cpp
        logic/King.h
        logic/Queen.cpp
        logic/Queen.h
        logic/Pawn.cpp
        logic/Pawn.h
        logic/Rook.cpp
        logic/Rook.h
        logic/Bishop.cpp
        logic/Bishop.h
        logic/Knight.cpp
        logic/Knight.h
        common/function.cpp
        view/BoardBase.h view/Piece.h view/Piece.cpp)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)  # Mettre ici ce qui est requis de Qt (et mettre Qt5 ou Qt6 selon la version que vous avez).
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)  # Doit être après le "add_executable" et lui passer le même nom.  Mettre encore ici ce qui est requis de Qt.