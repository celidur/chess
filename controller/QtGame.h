
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
        void displayMessage(std::string msg) override; // Addition to display the message in a system box

    public:
        QtGame();

        explicit QtGame(const TypePiece board[8][8], Color color = Color::white);

        ~QtGame() override;

    signals:

        QEvent *displayQtMessage(const QString &s);

    public slots:

        void updateGameState(Coord coord, screen::Board &board);

        void updateGameState(TypePiece promotePiece, screen::Board &board);

        void loadGame(Color color) override;

        void resetBoard() override;

        void setDefaultBoard() override;

        void addPiece(TypePiece typePiece, Coord pos, screen::Board &board);
    };
}
#endif //CHESS_QTGAME_H
