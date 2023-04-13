#include "Piece.h"

namespace chess {

    Piece::Piece(const Coord &pos, Color color) : pos_(pos), color_(color) {}

    bool Piece::move(const TypePiece board[8][8], const Coord &pos) {
        for (const auto& move: possibleMoves_) {
            if (move == pos) {
                if (board[pos.x][pos.y].type != Type::none) {
                    board[pos.x][pos.y].piece->kill();
                }
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
                TypePiece boardCopy[8][8];
                copyBoard(board, boardCopy, pos, pos_);
                if (color_ == Color::white) {
                    if (whiteKing != nullptr && !whiteKing->isLegalMove(boardCopy, whiteKing->pos_)) {
                        continue;
                    }
                } else {
                    if (blackKing != nullptr && !blackKing->isLegalMove(boardCopy, blackKing->pos_)) {
                        continue;
                    }
                }
                possibleMoves_.push_back(pos);
            }
        }
    }

    void Piece::reset() {
        whiteKing = nullptr;
        blackKing = nullptr;
    }

}