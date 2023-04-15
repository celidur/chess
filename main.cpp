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

    game.updateBoard(board);

    auto boardView = QGraphicsView(&board);
    boardView.setFixedSize(screenSize.x, screenSize.y);
    boardView.window()->setWindowTitle("Chess!");
    boardView.show();

    return app.exec();
}