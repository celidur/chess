/**
* \file   QtGame.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "QtGame.h"

namespace controller {

    QtGame::QtGame() : QObject(nullptr) {}

    QtGame::~QtGame() = default;

    void QtGame::doUpdateGame(Coord& coord) {
        selectionCase(coord);
        if (mode_ == Mode::personalisation) {
            emit updatePersonalizationMenu();
        }
    }

    void QtGame::doPromotePiece(TypePiece& promotePiece) {
        auto pos = promotionPos_;
        promotion(promotePiece.type);
        emit addPieceQt(promotePiece, pos);
    }

    void QtGame::doAddPiece(TypePiece& typePiece, Coord& pos) {
        addPiece(pos, typePiece);
        emit addPieceQt(typePiece, pos);
    }

    void QtGame::doLoadGame() {
        loadGame();
        updateBoard();
    }

    void QtGame::doSwitchPlayer(Color color) {
        setPlayerRound(color);
        emit updatePiece();
        emit updatePersonalizationMenu();
    }

    void QtGame::doSetDefaultBoard() {
        setDefaultBoard(board_);
        updateBoard();
    }

    void QtGame::doResetBoard() {
        resetBoard(board_);
        updateBoard();
    }

    void QtGame::displayMessage(const std::string& msg) {
        emit displayQtMessage(QString::fromStdString(msg));
    }

    void QtGame::doSwitchRotation() {
        setRotation(!isRotation());
        emit updatePersonalizationMenu();
    }

    void QtGame::updateGameBoard(Coord* selection, TypePiece boardGame[xBoard][yBoard],
                                 std::vector<Coord>& piecePossibleMove,
                                 Color color, std::vector<TypePiece>* deadPieces, int point) {
        emit updateGameQt(selection, boardGame, piecePossibleMove, color, deadPieces, point);
    }

    void QtGame::viewBoard(Color color) {
        emit viewBoardQt(color);
    }

    void QtGame::updatePersonalizationBoard(TypePiece boardGame[xBoard][yBoard]) {
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

    void QtGame::killPiece(Coord& pos) {
        updatePanel();
        emit killPieceQt(pos);
    }

    void QtGame::movePiece(Coord& pos1, Coord& pos2) {
        updatePanel();
        emit movePieceQt(pos1, pos2);
    }

    void QtGame::updateSelection(Coord& pos, std::vector<Coord>& piecePossibleMove) {
        emit updateSelectionQt(pos, piecePossibleMove);
    }

    void QtGame::updateCheck() {
        emit updateCheckStateQt(selection_[3]);
    }

    void QtGame::updatePanel() {
        std::vector<TypePiece> deadPieces[2] = {player_[0].getDeadPieces(), player_[1].getDeadPieces()};
        int points = getPieceValue(deadPieces[0]) - getPieceValue(deadPieces[1]);
        emit updatePanelQt(deadPieces, points);
    }

    void QtGame::showPromotion() {
        emit showPromotionQt(playerRound_);
    }
}
