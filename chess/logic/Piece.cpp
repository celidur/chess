/**
* \file   Piece.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Piece.h"

namespace logic {

    Piece::Piece(const Coord& pos, const TypePiece type) : pos_(pos), type_(type), promotion_(false) {}

    bool Piece::move(const TypePiece board[xBoard][yBoard], const Coord& pos) {
        for (auto move: possibleMoves_) {
            if (move == pos) {
                pos_ = pos;
                return true;
            }
        }
        return false;
    }

    void Piece::update(const TypePiece board[xBoard][yBoard]) {
        possibleMoves_.clear();
        for (const auto& move: legalMoves_) {
            Coord pos = pos_ + move;
            if (isLegalMove(board, pos))
                possibleMoves_.push_back(pos);
        }
    }

    void Piece::reset() {
        whiteKing = nullptr;
        blackKing = nullptr;
    }

    std::vector<Coord> Piece::getPossibleMoves() const { return possibleMoves_; }

    Coord Piece::getPos() const { return pos_; }

    void Piece::setMove(std::vector<Coord>& move) { possibleMoves_ = move; }

    void Piece::setPromotion() { promotion_ = true; }

    bool Piece::getPromotion() const { return promotion_; }

    bool Piece::isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos) {
        if (pos < 0 || pos >= Coord{xBoard, yBoard} || pos == pos_) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (piece.type != Type::none && piece.color == type_.color) {
            return false;
        }
        return true;
    }

    TypePiece Piece::getType() {
        return type_;
    }
}