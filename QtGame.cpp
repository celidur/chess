#include "QtGame.h"

namespace chess {
    QtGame::QtGame() : QObject(nullptr) {}

    void QtGame::updateGameState(Coord coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    QtGame::QtGame(const screen::TypePiece (*board)[8], Colour color) : QObject(nullptr), Game(board, color) {

    }

    QtGame::~QtGame() = default;
}
