cmake_minimum_required(VERSION 3.25)
project(chess)
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()
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
        view/BoardBase.h
        view/Piece.h
        view/Piece.cpp
        view/InterfaceModeState.h
        view/GameMode.cpp
        view/GameMode.h
        view/PersonnalisationMode.cpp
        view/PersonnalisationMode.h
        common/struct.h
        common/function.cpp
        controller/QtGame.cpp
        controller/QtGame.h
        logic/Piece.cpp
        logic/Player.cpp
        logic/Game.h
        logic/Game.cpp
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
        logic/CheckChess.h
        logic/CheckChess.cpp
        view/StateFactory.h view/IBoard.cpp view/IBoard.h)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/chess/res ${CMAKE_BINARY_DIR}/res)
