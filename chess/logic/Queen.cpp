/**
* \file   Queen.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Queen.h"

namespace logic {
    Queen::Queen(const Coord &pos, Color color) : Piece(pos, color), Rook(pos, color), Bishop(pos, color) {}

    bool Queen::isLegalMove(const TypePiece board[8][8], Coord pos) {
        return Rook::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
    }

    TypePiece Queen::getType() {
        return {color_, Type::queen};
    }
}