
#ifndef CHESS_QTGAME_H
#define CHESS_QTGAME_H

#include <QObject>
#include <Game.h>
#include <Board-qt.h>


namespace chess {
    class QtGame : public QObject, public chess::Game{
        Q_OBJECT
    public:
        QtGame();

        ~QtGame() override;

    public slots:
        void updateGameState(Coord coord, screen::Board& board);

    };

}

#endif //CHESS_QTGAME_H
