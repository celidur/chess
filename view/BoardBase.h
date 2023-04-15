//
// Created by Gaetan on 2023-04-15.
//

#ifndef CHESS_BOARDBASE_H
#define CHESS_BOARDBASE_H

#include "common/struct.h"

namespace view {


    class BoardBase {
    public:
        virtual ~BoardBase() = default;

        virtual void updateGame(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color, std::vector<TypePiece> deadPieces[2]) = 0;

        virtual void updatePersonalization(TypePiece boardGame[8][8]) = 0;

        virtual void viewBoard(Color color) = 0;
    };
}

#endif //CHESS_BOARDBASE_H
