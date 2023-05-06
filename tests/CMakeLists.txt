cmake_minimum_required(VERSION 3.25)
project(chess-tests)
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
set(CMAKE_CXX_STANDARD 20)


include_directories(../chess)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(../chess)

add_executable(${PROJECT_NAME}
        logic-tests.cpp
        ../chess/logic/Piece.cpp
        ../chess/logic/Player.cpp
        ../chess/logic/Game.h
        ../chess/logic/Game.cpp
        ../chess/common/struct.h
        ../chess/logic/King.cpp
        ../chess/logic/King.h
        ../chess/logic/Queen.cpp
        ../chess/logic/Queen.h
        ../chess/logic/Pawn.cpp
        ../chess/logic/Pawn.h
        ../chess/logic/Rook.cpp
        ../chess/logic/Rook.h
        ../chess/logic/Bishop.cpp
        ../chess/logic/Bishop.h
        ../chess/logic/Knight.cpp
        ../chess/logic/Knight.h
        ../chess/common/function.cpp
        ../chess/logic/CheckChess.cpp
        ../chess/logic/CheckChess.h
        ../chess/controller/QtGame.h
        ../chess/controller/QtGame.cpp
        ../chess/view/Board.cpp
        ../chess/view/Board.h
        ../chess/view/BoardBase.h
        ../chess/view/GameMode.cpp
        ../chess/view/GameMode.h
        ../chess/view/IBoard.cpp
        ../chess/view/IBoard.h
        ../chess/view/InterfaceModeState.h
        ../chess/view/PersonnalisationMode.cpp
        ../chess/view/Piece.cpp
        ../chess/view/Piece.h
        ../chess/view/PersonnalisationMode.h
        ../chess/view/StateFactory.h
        )
target_link_libraries(${PROJECT_NAME} gtest_main)
add_test(NAME BaseTests COMMAND base)

# Pour Google Test (basé sur https:://stackoverflow.com/questions/1620918/cmake-and-libpthread )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
add_compile_definitions(_POSIX_C_SOURCE=200809L)  # Requis pour compiler GoogleTest sous Cygiwin, voir https::/chess/github.com/google/googletest/issues/813

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)