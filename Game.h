//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Player.h"
#include "Board.h"

namespace chess {
    class Game {
    public:
        Game();

        ~Game() = default;

        void update();

        void selectionCase(Coord pos);

        void updateBoard(screen::Board &board);

    private:
        std::vector<Player> player_;
        TypePiece board_[8][8];
        Colour playerRound_;
        Piece *pieceSelected_;
        Coord selection_[4];
    };
}

#endif //CHESS_GAME_H
