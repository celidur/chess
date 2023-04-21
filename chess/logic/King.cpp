/**
* \file   King.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "King.h"

namespace logic {

    King::King(const Coord& pos, Color color) : Piece(pos, color) {
        if (color == Color::white) {
            if (whiteKing != nullptr)
                throw std::runtime_error("White king already exists");
            whiteKing = this;
        } else {
            if (blackKing != nullptr)
                throw std::runtime_error("Black king already exists");
            blackKing = this;
        }
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (i == 0 && j == 0)
                    continue;
                legalMoves_.emplace_back(Coord{i, j});
            }
        }
        legalMoves_.emplace_back(Coord{2, 0});
        legalMoves_.emplace_back(Coord{-2, 0});
        int kingLine = color == Color::white ? 0 : yBoard - 1;
        first_ = pos == Coord{3, kingLine};
    }

    bool King::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        auto res = Piece::isLegalMove(board, pos);
        if (!res) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (abs(pos.x - pos_.x) > 2 || abs(pos.y - pos_.y) > 1) {
            return false;
        }
        if (abs(pos.x - pos_.x) == 2) {
            if (!first_ || piece.type != Type::none || pos.y != pos_.y)
                return false;
            int direction = pos.x - pos_.x == 2 ? -1 : 1;
            if (board[pos.x + direction][pos.y].type != Type::none)
                return false;
            if (direction == -1 && board[pos_.x + 2][pos.y].type != Type::none)
                return false;
            auto rook = board[(direction == -1 ? 4 : -3) + pos_.x][pos.y];
            if (rook.type != Type::rook || rook.color != color_ || !rook.first)
                return false;
        }
        return true;
    }

    bool King::move(const TypePiece board[xBoard][yBoard], const Coord& pos) {
        bool res = Piece::move(board, pos);
        if (!res)
            return false;
        first_ = false;
        return true;
    }

    void King::update(const TypePiece board[xBoard][yBoard]) {
        possibleMoves_.clear();
        for (auto& move: legalMoves_) {
            auto pos = Coord{pos_.x + move.x, pos_.y + move.y};
            if (isLegalMove(board, pos)) {
                possibleMoves_.emplace_back(pos);
            }
        }
    }

    TypePiece King::getType() {
        return {color_, Type::king};
    }
}