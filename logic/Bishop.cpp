/**
* \file   Bishop.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Bishop.h"

namespace logic{
    Bishop::Bishop(const Coord &pos, Colour color) : Piece(pos, color) {
        for (int i = 0; i < 8; i++) {
            legalMoves_.emplace_back(Coord{i, i});
            legalMoves_.emplace_back(Coord{-i, i});
            legalMoves_.emplace_back(Coord{i, -i});
            legalMoves_.emplace_back(Coord{-i, -i});
        }
    }

    bool Bishop::isLegalMove(const TypePiece board[8][8], Coord pos) {
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
        if (abs(pos.x - pos_.x) != abs(pos.y - pos_.y)) {
            return false;
        }
        int x = pos.x - pos_.x;
        int y = pos.y - pos_.y;
        int i = 1;
        while (i < abs(x)) {
            if (board[pos_.x + i * x / abs(x)][pos_.y + i * y / abs(y)].type != Type::none) {
                return false;
            }
            i++;
        }
        return true;
    }

    TypePiece Bishop::getType() {
        return {color_, Type::bishop, this};
    }
}