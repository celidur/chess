#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H
#include "Piece.h"

namespace chess{
    class Bishop : virtual public Piece {
    public:
        Bishop(const Coord& pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };
}


#endif //CHESS_BISHOP_H
