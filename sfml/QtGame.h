
#ifndef CHESS_QTGAME_H
#define CHESS_QTGAME_H

#include <QObject>
#include <QMessageBox>
#include "logic/Game.h"
#include "vue/Board.h"


namespace chess {
    class QtGame : public QObject, public chess::Game{
        Q_OBJECT
    protected:
//        void displayMessage(std::string msg) override; // Addition to display the message in a system box

    public:
        QtGame();

        explicit QtGame(const screen::TypePiece board[8][8], Colour color = Colour::white);

        ~QtGame() override;

    public slots:
        void updateGameState(Coord coord, screen::Board& board);
        void updateGameState(screen::TypePiece promotePiece, screen::Board& board);
    };
}

#endif //CHESS_QTGAME_H
