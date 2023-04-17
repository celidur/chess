/**
* \file   QtGame.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "QtGame.h"

namespace controller {

    QtGame::QtGame() : QObject(nullptr) {}
    QtGame::QtGame(const TypePiece (*board)[8], Color color) : QObject(nullptr), Game(board, color) {}
    QtGame::~QtGame() = default;

    void QtGame::doUpdateGame(Coord& coord) {
        selectionCase(coord);
        updateBoard();
    }

    void QtGame::doPromotePiece(TypePiece& promotePiece) {
        promotion(promotePiece.type);
        updateBoard();
    }

    void QtGame::doAddPiece(TypePiece& typePiece, Coord& pos){
        addPiece(pos, typePiece);
        updateBoard();
    }

    void QtGame::doLoadGame(){
        loadGame();
        updateBoard();
    }

    void QtGame::doSwitchPlayer(Color color) {
        setPlayerRound(color);
        updateBoard();
    }

    void QtGame::doSetDefaultBoard() {
        setDefaultBoard();
        updateBoard();
    }

    void QtGame::doResetBoard() {
        resetBoard();
        updateBoard();
    }

    void QtGame::displayMessage(const std::string& msg) {
        emit displayQtMessage(QString::fromStdString(msg));
    }

    void QtGame::doSwitchRotation() {
        setRotation(!isRotation());
        updateBoard();
    }

    void QtGame::updateGameBoard(Coord *selection, TypePiece (*boardGame)[8], std::vector<Coord> &piecePossibleMove,
                                 Color color, std::vector<TypePiece> *deadPieces, int point) {
        emit updateGameQt(selection, boardGame, piecePossibleMove, color, deadPieces, point);
    }

    void QtGame::viewBoard(Color color) {
        emit viewBoardQt(color);
    }

    void QtGame::updatePersonalizationBoard(TypePiece boardGame[8][8]) {
        emit updatePersonalizationQt(boardGame);
    }


    void QtGame::updateBoard() {
        if (mode_ == Mode::game) {
            if (rotation_)
                viewBoard(playerRound_);
            auto movePossible = player_[(int) playerRound_].getPossibleMoves(selection_[0]);
            std::vector<TypePiece> deadPieces[2] = {player_[0].getDeadPieces(), player_[1].getDeadPieces()};
            int points = getPieceValue(deadPieces[0]) - getPieceValue(deadPieces[1]);
            updateGameBoard(selection_, board_, movePossible,
                            promotionPos_ != Coord{-1, -1} ? playerRound_ : Color::none, deadPieces, points);
        } else {
            updatePersonalizationBoard(board_);
        }
    }
}
