
#ifndef CHESS_QTGAME_H
#define CHESS_QTGAME_H

#include <QObject>
#include <QMessageBox>
#include "logic/Game.h"
#include "view/Board.h"


namespace chess {
    class QtGame : public QObject, public chess::Game {
    Q_OBJECT

    protected:
        void displayMessage(const std::string &msg) override; // Addition to display the message in a system box

    public:
        QtGame();

        explicit QtGame(const TypePiece board[8][8], Color color = Color::white);

        ~QtGame() override;

    signals:

        QEvent *displayQtMessage(const QString &s);

    public slots:

        void doUpdateGame(Coord &coord, screen::Board &board);

        void doPromotePiece(TypePiece &promotePiece, screen::Board &board);

        void doAddPiece(TypePiece &typePiece, Coord &pos, screen::Board &board);

        void doLoadGame(screen::Board &board);

        void doSwitchPlayer(Color color, screen::Board &board);

        void doSetDefaultBoard(screen::Board &board);

        void doResetBoard(screen::Board &board);

        void doSwitchRotation(screen::Board& board);
    };
}
#endif //CHESS_QTGAME_H
