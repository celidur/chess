/*
* Travail : $FOLDER_NAME-QtGame.cpp
* Section # : 01
* Équipe # : 11
* Auteurs: Charles Khoury, Thomas Tzeng Souvanlasy
* Correcteur : -------
*/

#include "QtGame.h"

namespace chess {
    QtGame::QtGame() : QObject(nullptr) {}

    void QtGame::updateGameState(Coord coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    QtGame::~QtGame() = default;

}
