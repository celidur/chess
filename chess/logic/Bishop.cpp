/**
* \file   Bishop.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Bishop.h"

namespace logic {
    Bishop::Bishop(const Coord& pos, Color color) : Piece(pos, color) {
        for (int i = 0; i < maxBoard; i++) {
            legalMoves_.emplace_back(Coord{i, i});
            legalMoves_.emplace_back(Coord{-i, i});
            legalMoves_.emplace_back(Coord{i, -i});
            legalMoves_.emplace_back(Coord{-i, -i});
        }
    }

    bool Bishop::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        auto res = Piece::isLegalMove(board, pos);
        if (!res) {
            return false;
        }
        if (abs(pos.x - pos_.x) != abs(pos.y - pos_.y)) {
            return false;
        }
        int x = pos.x - pos_.x;
        int y = pos.y - pos_.y;
        for (int i = 1; i < abs(x); ++i) {
            if (board[pos_.x + i * x / abs(x)][pos_.y + i * y / abs(y)].type != Type::none)
                return false;
        }
        return true;
    }

    TypePiece Bishop::getType() {
        return {color_, Type::bishop};
    }
}