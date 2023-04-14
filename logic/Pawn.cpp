#include "Pawn.h"

namespace chess{


    Pawn::Pawn(const Coord& pos, Color color) : Piece(pos, color) {
        int direction = color == Color::white ? 1 : -1;
        int pawnLine = color == Color::white ? 1 : 6;
        first_ = pos.y == pawnLine ? 0 : -1;
        legalMoves_.emplace_back(Coord{0, direction});
        legalMoves_.emplace_back(Coord{0, direction * 2});
        legalMoves_.emplace_back(Coord{1, direction});
        legalMoves_.emplace_back(Coord{-1, direction});
    }

    bool Pawn::isLegalMove(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], Coord pos) {
        int direction = color_ == Color::white ? 1 : -1;
        auto piece = board[pos.x][pos.y];
        if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
            return false;
        }
        if (pos.x == pos_.x && pos.y == pos_.y + direction) {
            return piece.type == Type::none;
        }
        if (pos.x == pos_.x && pos.y == pos_.y + 2 * direction && first_ == 0) {
            return piece.type == Type::none && board[pos.x][pos.y - direction].type == Type::none;
        }
        if ((pos.x == pos_.x + 1 || pos.x == pos_.x - 1) && pos.y == pos_.y + direction) {
            if (piece.type != Type::none)
                return piece.color != color_; // for eating
            auto pawn = dynamic_cast<Pawn *>(pieceBoard[pos.x][pos_.y].get());
            if (pawn == nullptr)
                return false;
            if (pawn->color_ != color_ && pawn->first_ == 1)
                return true;
        }
        return false;
    }

    bool Pawn::move(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], const Coord &pos) {
        Coord posCopy = pos_;
        bool res = Piece::move(board, nullptr, pos);
        if (!res)
            return false;
        if ((pos.x == posCopy.x + 1 || pos.x == posCopy.x - 1) && board[pos.x][pos.y].type == Type::none) {
            pieceBoard[pos.x][posCopy.y]->kill();
        }
        int direction = color_ == Color::white ? 2 : -2;
        first_ = (posCopy.y + direction == pos.y) ? 1 : -1;
        return true;
    }

    void Pawn::update(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]) {
        Piece::update(board, pieceBoard);
        first_ = (first_ == 1 | first_ == -1) ? -1 : 0;
    }

    TypePiece Pawn::getType() {
        return {color_, Type::pawn, first_};
    }
}
