#include "Queen.h"

namespace chess {
    Queen::Queen(const Coord &pos, Color color) : Piece(pos, color), Rook(pos, color), Bishop(pos, color) {}

    bool Queen::isLegalMove(const TypePiece board[8][8], Coord pos) {
        return Rook::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
    }

    TypePiece Queen::getType() {
        return {color_, Type::queen};
    }
}