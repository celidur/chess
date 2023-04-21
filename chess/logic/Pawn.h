/**
* \file   Pawn.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

namespace logic {
    class Pawn : public Piece {
    public:
        Pawn(const Coord& pos, Color color);

        void update(const TypePiece board[xBoard][yBoard]) override;

        bool move(const TypePiece board[xBoard][yBoard], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        int first_;
    };

}

#endif //CHESS_PAWN_H
