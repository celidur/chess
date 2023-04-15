/**
* \file   Rook.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Rook.h"

namespace chess {
    Rook::Rook(const Coord &pos, Color color) : Piece(pos, color) {
        for (int i = 0; i < 8; i++) {
            legalMoves_.emplace_back(Coord{i, 0});
            legalMoves_.emplace_back(Coord{0, i});
            legalMoves_.emplace_back(Coord{-i, 0});
            legalMoves_.emplace_back(Coord{0, -i});
        }
    }

    bool Rook::isLegalMove(const TypePiece board[8][8], Coord pos) {
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
        if (pos.x != pos_.x && pos.y != pos_.y) {
            return false;
        }
        int x = pos.x - pos_.x;
        int y = pos.y - pos_.y;
        int i = 1;
        while (i < abs(x) + abs(y)) {
            if (x != 0) {
                if (board[pos_.x + i * x / abs(x)][pos.y].type != Type::none) {
                    return false;
                }
            } else {
                if (board[pos_.x][pos_.y + i * y / abs(y)].type != Type::none) {
                    return false;
                }
            }
            i++;
        }
        return true;
    }

    bool Rook::move(const TypePiece board[8][8], const Coord &pos) {
        bool res = Piece::move(board, pos);
        if (res)
            first_ = false;
        return res;
    }

    TypePiece Rook::getType() {
        return {color_, Type::rook, first_};
    }
}