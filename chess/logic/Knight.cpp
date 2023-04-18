/**
* \file   Knight.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Knight.h"

namespace logic {
    Knight::Knight(const Coord& pos, Color color) : Piece(pos, color) {
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
        if (pos < 0 || pos > 7 || pos == pos_) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (piece.type != Type::none && piece.color == color_) {
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