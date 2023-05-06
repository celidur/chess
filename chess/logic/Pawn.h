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
        Pawn(const Coord& pos, const Color& color);

        void update(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) override;

        bool move(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) override;

        bool isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) override;
    };

}

#endif //CHESS_PAWN_H
