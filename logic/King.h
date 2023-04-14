#ifndef CHESS_KING_H
#define CHESS_KING_H
#include "Piece.h"

namespace chess {

    class King : public Piece {
    public:
        King(const Coord &pos, Color color);

        void update(const TypePiece board[8][8]) override;

        bool move(const TypePiece board[8][8], const Coord &pos) override;

        bool isLegalMove(const TypePiece board[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;

    private:
        bool first_ = true;
    };

}


#endif //CHESS_KING_H
