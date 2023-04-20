/**
* \file   Knight.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "common/struct.h"
#include "Piece.h"

namespace logic {
    class Knight : public Piece {
    public:
        Knight(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    };

}


#endif //CHESS_KNIGHT_H
