#include "Queen.h"

namespace chess {
    Queen::Queen(const Coord &pos, Color color) : Piece(pos, color), Rook(pos, color), Bishop(pos, color) {}

    bool Queen::isLegalMove(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], Coord pos) {
        return Rook::isLegalMove(board, nullptr, pos) || Bishop::isLegalMove(board, nullptr, pos);
    }

    TypePiece Queen::getType() {
        return {color_, Type::queen};
    }
}