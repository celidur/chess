/**
* \file   King.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"

namespace logic {

    class King : public Piece {
    public:
        King(const Coord& pos, const Color& color);

        void update(const TypePiece board[xBoard][yBoard]) override;

        bool move(const TypePiece board[xBoard][yBoard], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[xBoard][yBoard], const Coord& pos) override;
    };

}


#endif //CHESS_KING_H
