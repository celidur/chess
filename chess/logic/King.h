/**
* \file   King.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"

namespace logic {

    class King : public Piece {
    public:
        King(const Coord& pos, Color color);

        void update(const TypePiece board[xBoard][yBoard]) override;

        bool move(const TypePiece board[xBoard][yBoard], const Coord& pos) override;

        bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };

}


#endif //CHESS_KING_H
