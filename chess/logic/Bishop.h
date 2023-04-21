/**
* \file   Bishop.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

namespace logic {
    class Bishop : virtual public Piece {
    public:
        Bishop(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };
}


#endif //CHESS_BISHOP_H