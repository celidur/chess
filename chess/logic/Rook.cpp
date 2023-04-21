/**
* \file   Rook.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Rook.h"

namespace logic {
    Rook::Rook(const Coord& pos, Color color) : Piece(pos, color) {
        for (int i = 0; i < maxBoard; i++) {
            legalMoves_.emplace_back(Coord{i, 0});
            legalMoves_.emplace_back(Coord{0, i});
            legalMoves_.emplace_back(Coord{-i, 0});
            legalMoves_.emplace_back(Coord{0, -i});
        }
    }

    bool Rook::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        auto res = Piece::isLegalMove(board, pos);
        if (!res) {
            return false;
        }
        if (pos.x != pos_.x && pos.y != pos_.y) {
            return false;
        }
        int x = pos.x - pos_.x;
        int y = pos.y - pos_.y;
        for (int i = 1; i < abs(x) + abs(y); ++i) {
            if (x != 0) {
                if (board[pos_.x + i * x / abs(x)][pos.y].type != Type::none) {
                    return false;
                }
            } else {
                if (board[pos_.x][pos_.y + i * y / abs(y)].type != Type::none) {
                    return false;
                }
            }
        }
        return true;
    }

    bool Rook::move(const TypePiece board[xBoard][yBoard], const Coord& pos) {
        bool res = Piece::move(board, pos);
        if (res)
            first_ = false;
        return res;
    }

    TypePiece Rook::getType() {
        return {color_, Type::rook, first_};
    }
}