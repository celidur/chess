/**
* \file   Knight.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "common/struct.h"
#include "Piece.h"

namespace logic {
    class Knight : public Piece {
    public:
        Knight(const Coord& pos, const Color& color);

        bool isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) override;

    };

}


#endif //CHESS_KNIGHT_H
