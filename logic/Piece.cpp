#include "Piece.h"

namespace chess {

    Piece::Piece(const Coord &pos, Color color) : pos_(pos), color_(color), isAlive_(true) {}

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