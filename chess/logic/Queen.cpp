/**
* \file   Queen.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Queen.h"

namespace logic {
    Queen::Queen(const Coord& pos, const Color& color) : Piece(pos, {color, Type::queen}), Rook(pos, color), Bishop(pos, color) {}

    bool Queen::isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) {
        return Rook::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
    }
}