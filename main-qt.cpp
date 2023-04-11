#include <Board-qt.h>
#include <QtGame.h>
#include <QApplication>
#include <QGraphicsView>

constexpr screen::CoordF tileSize{72, 72};
constexpr screen::CoordF screenSize{tileSize.x * 8.03, tileSize.y * 8.03};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    chess::QtGame game;

    screen::Board board(tileSize, "res/chess.png", game.getBoard());
    QWidget::connect(
            &board,
            SIGNAL(caseClicked(Coord, screen::Board & )),
            &game,
            SLOT(updateGameState(Coord, screen::Board & )));
    //add promotion menu
    QWidget::connect(
            &board,
            SIGNAL(promoteClicked(screen::TypePiece, screen::Board & )),
            &game,
            SLOT(updateGameState(screen::TypePiece, screen::Board & )));

    auto boardView = QGraphicsView(&board);
    boardView.setFixedSize(screenSize.x, screenSize.y);
    boardView.window()->setWindowTitle("Chess!");
    boardView.show();

    return app.exec();
}