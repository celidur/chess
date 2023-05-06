/**
* \file   Piece.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Piece.h"

namespace logic {

    Piece::Piece(const Coord& pos, const TypePiece type) : pos_(pos), type_(type), promotion_(false) {}

    bool Piece::move(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) {
        for (auto move: possibleMoves_) {
            if (move == pos) {
                pos_ = pos;
                return true;
            }
        }
        return false;
    }

    void Piece::update(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) {
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

    std::vector<Coord>& Piece::getPossibleMoves() { return possibleMoves_; }

    Coord Piece::getPos() const { return pos_; }

    void Piece::setPromotion() { promotion_ = true; }

    bool Piece::getPromotion() const { return promotion_; }

    bool Piece::isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos) {
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