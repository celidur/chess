/**
* \file   Queen.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "Rook.h"
#include "Bishop.h"
#include "common/struct.h"

namespace logic {
    class Queen : public Rook, Bishop {
    public:
        Queen(const Coord& pos, const Color& color);

        bool isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) override;
    };

}

#endif //CHESS_QUEEN_H
