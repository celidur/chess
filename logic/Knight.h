#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "common/struct.h"
#include "Piece.h"

namespace chess {
    class Knight : public Piece {
    public:
        Knight(const Coord &pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    };

}


#endif //CHESS_KNIGHT_H
