#include "QtGame.h"

namespace chess {
    QtGame::QtGame() : QObject(nullptr) {}

    void QtGame::updateGameState(Coord coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    QtGame::QtGame(const screen::TypePiece (*board)[8], Color color) : QObject(nullptr), Game(board, color) {
    }

    void QtGame::updateGameState(screen::TypePiece promotePiece, screen::Board &board) {
        promotion(promotePiece.type);
        updateBoard(board);
    }

    QtGame::~QtGame() = default;
}
