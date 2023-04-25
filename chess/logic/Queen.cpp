/**
* \file   Queen.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Queen.h"

namespace logic {
    Queen::Queen(const Coord& pos, const TypePiece type) : Piece(pos, type), Rook(pos, type), Bishop(pos, type) {}

    bool Queen::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        return Rook::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
    }
}