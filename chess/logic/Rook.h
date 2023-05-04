/**
* \file   Rook.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"


namespace logic {
    class Rook : virtual public Piece {
    public:
        Rook(const Coord& pos, const Color& color);

        bool move(const TypePiece board[xBoard][yBoard], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[xBoard][yBoard], const Coord& pos) override;
    };
}


#endif //CHESS_ROOK_H
