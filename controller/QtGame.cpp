#include "QtGame.h"

namespace chess {

    QtGame::QtGame() : QObject(nullptr) {}
    QtGame::QtGame(const TypePiece (*board)[8], Color color) : QObject(nullptr), Game(board, color) {}
    QtGame::~QtGame() = default;

    void QtGame::doUpdateGame(Coord& coord, screen::Board& board) {
        selectionCase(coord);
        updateBoard(board);
    }

    void QtGame::doPromotePiece(TypePiece& promotePiece, screen::Board &board) {
        promotion(promotePiece.type);
        updateBoard(board);
    }

    void QtGame::doAddPiece(TypePiece& typePiece, Coord& pos, screen::Board& board){
        addPiece(pos, typePiece);
        updateBoard(board);
    }

    void QtGame::doLoadGame(screen::Board& board){
        loadGame();
        updateBoard(board);
    }

    void QtGame::doSwitchPlayer(Color color, screen::Board& board) {
        setPlayerRound(color);
        updateBoard(board);
    }

    void QtGame::doSetDefaultBoard(screen::Board &board) {
        setDefaultBoard();
        updateBoard(board);
    }

    void QtGame::doResetBoard(screen::Board &board) {
        resetBoard();
        updateBoard(board);
    }

    void QtGame::displayMessage(const std::string& msg) {
        emit displayQtMessage(QString::fromStdString(msg));
    }

    void QtGame::doSwitchRotation() {
        setRotation(!isRotation());
    }
}
