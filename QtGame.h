
#ifndef CHESS_QTGAME_H
#define CHESS_QTGAME_H

#include <QObject>
#include <QMessageBox>
#include <Game.h>
#include <Board.h>


namespace chess {
    class QtGame : public QObject, public chess::Game{
        Q_OBJECT
    protected:
//        void displayMessage(std::string msg) override; // Addition to display the message in a system box

    public:
        QtGame();

        explicit QtGame(const screen::TypePiece board[8][8], Color color = Color::white);

        ~QtGame() override;

    public slots:
        void updateGameState(Coord coord, screen::Board& board);
        void updateGameState(screen::TypePiece promotePiece, screen::Board& board);
        void loadGame(screen::Board& board);
    };
}

#endif //CHESS_QTGAME_H
