#include "QtGame.h"

namespace chess {

    QtGame::QtGame() : QObject(nullptr) {}
    QtGame::QtGame(const TypePiece (*board)[8], Color color) : QObject(nullptr), Game(board, color) {}
    QtGame::~QtGame() = default;

    void QtGame::updateGameState(Coord coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    void QtGame::updateGameState(TypePiece promotePiece, screen::Board &board) {
        promotion(promotePiece.type);
        updateBoard(board);
    }

    void QtGame::loadGame(Color color) {
        Game::loadGame(color);
    }

    void QtGame::resetBoard() {
        Game::resetBoard();
    }

    void QtGame::setDefaultBoard() {
        Game::setDefaultBoard();
    }

    void QtGame::displayMessage(const std::string& msg) {
        emit displayQtMessage(QString::fromStdString(msg));
    }

    void QtGame::addPiece(TypePiece typePiece, Coord pos, screen::Board& board){

    }
}
