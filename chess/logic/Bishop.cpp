/**
* \file   Bishop.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Bishop.h"

namespace logic {
    Bishop::Bishop(const Coord& pos, Color color) : Piece(pos, color) {
        for (int i = 0; i < 8; i++) {
            legalMoves_.emplace_back(Coord{i, i});
            legalMoves_.emplace_back(Coord{-i, i});
            legalMoves_.emplace_back(Coord{i, -i});
            legalMoves_.emplace_back(Coord{-i, -i});
        }
    }

    bool Bishop::isLegalMove(const TypePiece board[8][8], Coord pos) {
        if (pos < 0 || pos > 7 || pos == pos_) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (piece.type != Type::none && piece.color == color_) {
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