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

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    controller::QtGame game;
    game.update();
    view::Board board(tileSize,"../res/chess.png", game.getMode(), nullptr);

    game.connectSignals(board);

    game.updateBoard();

    auto boardView = QGraphicsView(&board);
    boardView.setFixedSize(screenSize.x, screenSize.y);
    boardView.window()->setWindowTitle("Chess");
    boardView.show();

    return QApplication::exec();
}