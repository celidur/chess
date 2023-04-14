#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H
#include "Rook.h"
#include "Bishop.h"
#include "common/struct.h"

namespace chess {
    class Queen : public Rook, Bishop {
    public:
        Queen(const Coord &pos, Color color);

        bool isLegalMove(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], Coord pos) override;

        [[nodiscard]] TypePiece getType() override;
    };

}

#endif //CHESS_QUEEN_H
