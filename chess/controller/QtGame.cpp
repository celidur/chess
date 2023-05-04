/**
* \file   QtGame.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
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

    void QtGame::doPromotePiece(const TypePiece& promotePiece) {
        auto pos = promotionPos_;
        promotion(promotePiece.type);
        emit addPieceQt(promotePiece, pos);
    }

    void QtGame::doAddPiece(const TypePiece& typePiece, const Coord& pos) {
        try {
            addPiece(pos, typePiece);
            emit addPieceQt(typePiece, pos);
        }catch (std::runtime_error& e) {
            displayMessage(e.what());
        }
    }

    void QtGame::doLoadGame() {
        loadGame();
        updateBoard();
    }

    void QtGame::doSwitchPlayer(const Color& color) {
        setPlayerRound(color);
        emit updatePiece();
        emit updatePersonalizationMenu();
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
        emit updatePersonalizationMenu();
    }

    void QtGame::updateGameBoard(Coord* selection, TypePiece boardGame[xBoard][yBoard],
                                 std::vector<Coord>& piecePossibleMove,
                                 Color color, std::vector<TypePiece>* deadPieces, int point) {
        emit updateGameQt(selection, boardGame, piecePossibleMove, color, deadPieces, point);
    }

    void QtGame::viewBoard(const Color& color) {
        emit viewBoardQt(color);
    }

    void QtGame::updatePersonalizationBoard(const TypePiece boardGame[xBoard][yBoard]) {
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

    void QtGame::killPiece(const Coord& pos) {
        updatePanel();
        emit killPieceQt(pos);
    }

    void QtGame::movePiece(Coord& pos1, Coord& pos2) {
        updatePanel();
        emit movePieceQt(pos1, pos2);
    }

    void QtGame::updateSelection(const Coord& pos, const std::vector<Coord>& piecePossibleMove) {
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

    void QtGame::connectSignals(const view::Board& board) const {
        QWidget::connect(
                &board,
                &view::Board::caseClicked,
                this,
                &QtGame::doUpdateGame);
        //add promotion menu
        QWidget::connect(
                &board,
                &view::Board::promoteClicked,
                this,
                &QtGame::doPromotePiece);
        QWidget::connect(
                &board,
                &view::Board::pieceAdded,
                this,
                &QtGame::doAddPiece);
        QWidget::connect(&board,
                         &view::Board::gameStarted,
                         this,
                         &QtGame::doLoadGame);
        QWidget::connect(&board,
                         &view::Board::playerSwitched,
                         this,
                         &QtGame::doSwitchPlayer);
        QWidget::connect(&board,
                         &view::Board::boardDefaulted,
                         this,
                         &QtGame::doSetDefaultBoard);
        QWidget::connect(&board,
                         &view::Board::boardReset,
                         this,
                         &QtGame::doResetBoard);
        QWidget::connect(&board,
                         &view::Board::rotationSwitched,
                         this,
                         &QtGame::doSwitchRotation);
        QWidget::connect(this,
                         &QtGame::displayQtMessage,
                         &board,
                         &view::Board::displayMessage);

        QWidget::connect(this,
                         &QtGame::updateGameQt,
                         &board,
                         &view::Board::updateGame);

        QWidget::connect(this,
                         &QtGame::updatePersonalizationQt,
                         &board,
                         &view::Board::updatePersonalization);

        QWidget::connect(this,
                         &QtGame::viewBoardQt,
                         &board,
                         &view::Board::viewBoard);

        QWidget::connect(this,
                         &QtGame::updatePersonalizationMenu,
                         &board,
                         &view::Board::updatePersonalizationMenu);

        QWidget::connect(this,
                         &QtGame::updatePiece,
                         &board,
                         &view::Board::updatePiece);

        QWidget::connect(this,
                         &QtGame::addPieceQt,
                         &board,
                         &view::Board::addPiece);

        QWidget::connect(this,
                         &QtGame::killPieceQt,
                         &board,
                         &view::Board::killPiece);

        QWidget::connect(this,
                         &QtGame::movePieceQt,
                         &board,
                         &view::Board::movePiece);

        QWidget::connect(this,
                         &QtGame::updateSelectionQt,
                         &board,
                         &view::Board::selectPiece);

        QWidget::connect(this,
                         &QtGame::updateCheckStateQt,
                         &board,
                         &view::Board::updateCheckState);

        QWidget::connect(this,
                         &QtGame::updatePanelQt,
                         &board,
                         &view::Board::updatePanel);

        QWidget::connect(this,
                         &QtGame::showPromotionQt,
                         &board,
                         &view::Board::promote);
    }
}
