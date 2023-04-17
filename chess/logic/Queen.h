/**
* \file   Queen.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H
#include "Rook.h"
#include "Bishop.h"
#include "common/struct.h"

namespace logic {
    class Queen : public Rook, Bishop {
    public:
        Queen(const Coord &pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };

}

#endif //CHESS_QUEEN_H
