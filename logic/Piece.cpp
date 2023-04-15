/**
* \file   Piece.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Piece.h"

namespace logic {

    Piece::Piece(const Coord &pos, Colour color) : pos_(pos), color_(color), isAlive_(true) {}

    bool Piece::move(const TypePiece board[8][8], const Coord &pos) {
        for (const auto& move: possibleMoves_) {
            if (move == pos) {
                pos_ = pos;
                return true;
            }
        }
        return false;
    }

    void Piece::update(const TypePiece board[8][8]) {
        possibleMoves_.clear();
        for (const auto &move: legalMoves_) {
            Coord pos = pos_ + move;
            if (isLegalMove(board, pos)) {
                possibleMoves_.push_back(pos);
            }
        }
    }

    void Piece::reset() {
        whiteKing = nullptr;
        blackKing = nullptr;
    }

    std::vector<Coord> Piece::getPossibleMoves() const { return possibleMoves_; }

    Coord Piece::getPos() const { return pos_; }

    void Piece::setMove(std::vector<Coord> &move) { possibleMoves_ = move; }
}