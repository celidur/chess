#include "Knight.h"

namespace chess {
    Knight::Knight(const Coord &pos, Color color) : Piece(pos, color) {
        legalMoves_.emplace_back(Coord{2, 1});
        legalMoves_.emplace_back(Coord{2, -1});
        legalMoves_.emplace_back(Coord{-2, 1});
        legalMoves_.emplace_back(Coord{-2, -1});
        legalMoves_.emplace_back(Coord{1, 2});
        legalMoves_.emplace_back(Coord{1, -2});
        legalMoves_.emplace_back(Coord{-1, 2});
        legalMoves_.emplace_back(Coord{-1, -2});
    }

    bool Knight::isLegalMove(const TypePiece board[8][8], Coord pos) {
        if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (piece.type != Type::none && piece.color == color_) {
            return false;
        }
        if (pos.x == pos_.x && pos.y == pos_.y) {
            return false;
        }
        if (abs(pos.x - pos_.x) == 2 && abs(pos.y - pos_.y) == 1) {
            return true;
        }
        if (abs(pos.x - pos_.x) == 1 && abs(pos.y - pos_.y) == 2) {
            return true;
        }
        return false;
    }

    TypePiece Knight::getType() {
        return {color_, Type::knight};
    }

}