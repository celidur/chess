/**
* \file   main.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "view/Board.h"
#include "controller/QtGame.h"
#include <QApplication>
#include <QGraphicsView>

constexpr CoordF tileSize{72, 72};
constexpr CoordF screenSize{tileSize.x * (xBoard+2), tileSize.y * yBoard};

void connectSignals(controller::QtGame& game, view::Board& board) {
    QWidget::connect(
            &board,
            &view::Board::caseClicked,
            &game,
            &controller::QtGame::doUpdateGame);
    //add promotion menu
    QWidget::connect(
            &board,
            &view::Board::promoteClicked,
            &game,
            &controller::QtGame::doPromotePiece);
    QWidget::connect(
            &board,
            &view::Board::pieceAdded,
            &game,
            &controller::QtGame::doAddPiece);
    QWidget::connect(&board,
                     &view::Board::gameStarted,
                     &game,
                     &controller::QtGame::doLoadGame);
    QWidget::connect(&board,
                     &view::Board::playerSwitched,
                     &game,
                     &controller::QtGame::doSwitchPlayer);
    QWidget::connect(&board,
                     &view::Board::boardDefaulted,
                     &game,
                     &controller::QtGame::doSetDefaultBoard);
    QWidget::connect(&board,
                     &view::Board::boardReset,
                     &game,
                     &controller::QtGame::doResetBoard);
    QWidget::connect(&board,
                     &view::Board::rotationSwitched,
                     &game,
                     &controller::QtGame::doSwitchRotation);
    QWidget::connect(&game,
                     &controller::QtGame::displayQtMessage,
                     &board,
                     &view::Board::displayMessage);

    QWidget::connect(&game,
                     &controller::QtGame::updateGameQt,
                     &board,
                     &view::Board::updateGame);

    QWidget::connect(&game,
                     &controller::QtGame::updatePersonalizationQt,
                     &board,
                     &view::Board::updatePersonalization);

    QWidget::connect(&game,
                     &controller::QtGame::viewBoardQt,
                     &board,
                     &view::Board::viewBoard);

    QWidget::connect(&game,
                     &controller::QtGame::updatePersonalizationMenu,
                     &board,
                     &view::Board::updatePersonalizationMenu);

    QWidget::connect(&game,
                     &controller::QtGame::updatePiece,
                     &board,
                     &view::Board::updatePiece);

    QWidget::connect(&game,
                     &controller::QtGame::addPieceQt,
                     &board,
                     &view::Board::addPiece);

    QWidget::connect(&game,
                     &controller::QtGame::killPieceQt,
                     &board,
                     &view::Board::killPiece);

    QWidget::connect(&game,
                     &controller::QtGame::movePieceQt,
                     &board,
                     &view::Board::movePiece);

    QWidget::connect(&game,
                     &controller::QtGame::updateSelectionQt,
                     &board,
                     &view::Board::selectPiece);

    QWidget::connect(&game,
                     &controller::QtGame::updateCheckStateQt,
                     &board,
                     &view::Board::updateCheckState);

    QWidget::connect(&game,
                     &controller::QtGame::updatePanelQt,
                     &board,
                     &view::Board::updatePanel);

    QWidget::connect(&game,
                     &controller::QtGame::showPromotionQt,
                     &board,
                     &view::Board::promote);
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    controller::QtGame game;
    game.update();
    view::Board board(tileSize,"../res/chess.png", game.getMode(), nullptr);

    connectSignals(game, board);

    game.updateBoard();

    auto boardView = QGraphicsView(&board);
    boardView.setFixedSize(screenSize.x, screenSize.y);
    boardView.window()->setWindowTitle("Chess");
    boardView.show();

    return QApplication::exec();
}