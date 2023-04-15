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

constexpr screen::CoordF tileSize{72, 72};
constexpr screen::CoordF screenSize{tileSize.x * 10, tileSize.y * 8};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    chess::QtGame game;

    screen::Board board(tileSize, "res/chess.png", game.getMode(), nullptr);

    QWidget::connect(
            &board,
            &screen::Board::caseClicked,
            &game,
            &chess::QtGame::doUpdateGame);
    //add promotion menu
    QWidget::connect(
            &board,
            &screen::Board::promoteClicked,
            &game,
            &chess::QtGame::doPromotePiece);
    QWidget::connect(
            &board,
            &screen::Board::pieceAdded,
            &game,
            &chess::QtGame::doAddPiece);
    QWidget::connect(&board,
                     &screen::Board::gameStarted,
                     &game,
                     &chess::QtGame::doLoadGame);
    QWidget::connect(&board,
                     &screen::Board::playerSwitched,
                     &game,
                     &chess::QtGame::doSwitchPlayer);
    QWidget::connect(&board,
                     &screen::Board::boardDefaulted,
                     &game,
                     &chess::QtGame::doSetDefaultBoard);
    QWidget::connect(&board,
                     &screen::Board::boardReset,
                     &game,
                     &chess::QtGame::doResetBoard);
    QWidget::connect(&board,
                     &screen::Board::rotationSwitched,
                     &game,
                     &chess::QtGame::doSwitchRotation);
    QWidget::connect(&game,
                     &chess::QtGame::displayQtMessage,
                     &board,
                     &screen::Board::displayMessage);

    QWidget::connect(&game,
                     &chess::QtGame::updateGameQt,
                     &board,
                     &screen::Board::updateGame);

    QWidget::connect(&game,
                     &chess::QtGame::updatePersonalizationQt,
                     &board,
                     &screen::Board::updatePersonalization);

    QWidget::connect(&game,
                     &chess::QtGame::viewBoardQt,
                     &board,
                     &screen::Board::viewBoard);

    game.updateBoard();

    auto boardView = QGraphicsView(&board);
    boardView.setFixedSize(screenSize.x, screenSize.y);
    boardView.window()->setWindowTitle("Chess");
    boardView.show();

    return QApplication::exec();
}