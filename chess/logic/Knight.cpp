/**
* \file   Knight.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Knight.h"

namespace logic {
    Knight::Knight(const Coord& pos, const Color& color) : Piece(pos, {color, Type::knight}) {
        legalMoves_.emplace_back(Coord{2, 1});
        legalMoves_.emplace_back(Coord{2, -1});
        legalMoves_.emplace_back(Coord{-2, 1});
        legalMoves_.emplace_back(Coord{-2, -1});
        legalMoves_.emplace_back(Coord{1, 2});
        legalMoves_.emplace_back(Coord{1, -2});
        legalMoves_.emplace_back(Coord{-1, 2});
        legalMoves_.emplace_back(Coord{-1, -2});
    }

    bool Knight::isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) {
        auto res = Piece::isLegalMove(board, pos);
        if (!res) {
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
}