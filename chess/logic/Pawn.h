/**
* \file   Pawn.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

namespace logic {
    class Pawn : public Piece {
    public:
        Pawn(const Coord& pos, Color color);

        void update(const TypePiece board[8][8]) override;

        bool move(const TypePiece board[8][8], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        int first_;
    };

}

#endif //CHESS_PAWN_H
