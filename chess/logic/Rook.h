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
        Rook(const Coord& pos, Color color);

        bool move(const TypePiece board[xBoard][yBoard], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };
}


#endif //CHESS_ROOK_H