/**
* \file   Pawn.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Pawn.h"

namespace logic {
    Pawn::Pawn(const Coord& pos, const TypePiece type) : Piece(pos, type) {
        int direction = type.color == Color::white ? 1 : -1;
        int pawnLine = type.color == Color::white ? 1 : yBoard - 2;
        type_.first = pos.y == pawnLine ? 0 : -1;
        legalMoves_.emplace_back(Coord{0, direction});
        legalMoves_.emplace_back(Coord{0, direction * 2});
        legalMoves_.emplace_back(Coord{1, direction});
        legalMoves_.emplace_back(Coord{-1, direction});
    }

    bool Pawn::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        int direction = type_.color == Color::white ? 1 : -1;
        auto piece = board[pos.x][pos.y];
        auto res = Piece::isLegalMove(board, pos);
        if (!res) {
            return false;
        }
        if (pos.x == pos_.x && pos.y == pos_.y + direction) {
            return piece.type == Type::none;
        }
        if (pos.x == pos_.x && pos.y == pos_.y + 2 * direction && type_.first == 0) {
            return piece.type == Type::none && board[pos.x][pos.y - direction].type == Type::none;
        }
        if (abs(pos.x - pos_.x) == 1 && pos.y == pos_.y + direction) {
            if (piece.type != Type::none)
                return piece.color != type_.color;
            auto pawn = board[pos.x][pos_.y];
            if (board[pos.x][pos_.y].type == Type::pawn && pawn.color != type_.color && pawn.first == 1)
                return true;
        }
        return false;
    }

    bool Pawn::move(const TypePiece board[xBoard][yBoard], const Coord& pos) {
        Coord posCopy = pos_;
        bool res = Piece::move(board, pos);
        if (!res)
            return false;
        int direction = type_.color == Color::white ? 2 : -2;
        type_.first = (posCopy.y + direction == pos.y) ? 1 : -1;
        return true;
    }

    void Pawn::update(const TypePiece board[xBoard][yBoard]) {
        Piece::update(board);
        type_.first = (type_.first == 1 | type_.first == -1) ? -1 : 0;
    }
}
