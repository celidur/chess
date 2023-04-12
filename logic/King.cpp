/*
* Travail : $FOLDER_NAME-King.cpp
* Section # : 01
* Équipe # : 11
* Auteurs: Charles Khoury, Thomas Tzeng Souvanlasy
* Correcteur : -------
*/

#include "King.h"
#include "Rook.h"

namespace chess{

    King::King(const Coord &pos, Color color) : Piece(pos, color) {
        if (color == Color::white) {
            if (whiteKing != nullptr)
                throw std::runtime_error("White king already exists");
            whiteKing = this;
        } else {
            if (blackKing != nullptr)
                throw std::runtime_error("Black king already exists");
            blackKing = this;
        }
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (i == 0 && j == 0)
                    continue;
                legalMoves_.emplace_back(Coord{i, j});
            }
        }
        legalMoves_.emplace_back(Coord{2, 0});
        legalMoves_.emplace_back(Coord{-2, 0});
        int kingLine = color == Color::white ? 0 : 7;
        first_ = pos == Coord{3, kingLine};
    }

    bool King::isLegalMove(const TypePiece board[8][8], Coord pos) {
        if (pos == pos_)
            return !isCheck(board);
        if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
            return false;
        }
        auto piece = board[pos.x][pos.y];
        if (piece.type != Type::none && piece.color == color_) {
            return false;
        }
        if (pos.x - pos_.x > 2 || pos.x - pos_.x < -2 || pos.y - pos_.y > 1 || pos.y - pos_.y < -1) {
            return false;
        }
        if (pos.x - pos_.x == 2 || pos.x - pos_.x == -2) {
            if (!first_ || isCheck(board) || piece.type != Type::none || pos.y != pos_.y)
                return false;
            int direction = pos.x - pos_.x == 2 ? -1 : 1;
            if (board[pos.x + direction][pos.y].type != Type::none)
                return false;
            if (direction == -1 && board[pos_.x + 2][pos.y].type != Type::none)
                return false;
            int xRook = (direction == -1 ? 4 : -3) + pos_.x;
            auto rook = board[xRook][pos.y];
            if (rook.type != Type::rook || rook.color != color_ || !rook.first)
                return false;
        }
        return true;
    }

    bool King::move(const TypePiece board[8][8], const Coord& pos) {
        Coord posCopy = pos_;
        bool res = Piece::move(board, pos);
        if (!res)
            return false;
        if (pos.x == posCopy.x + 2 && pos.y == posCopy.y) {
            auto rook = dynamic_cast<Rook *>(board[posCopy.x + 4][pos.y].piece);
            rook->move(board, {pos.x - 1, pos.y});
        }
        if (pos.x == posCopy.x - 2 && pos.y == posCopy.y) {
            auto rook = dynamic_cast<Rook *>(board[posCopy.x - 3][pos.y].piece);
            rook->move(board, {pos.x + 1, pos.y});
        }
        first_ = false;
        return true;
    }

    bool King::isCheck(const TypePiece board[8][8], Coord pos) {
        if (pos == Coord{-1, -1})
            pos = pos_;
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                if (board[i][j].type == Type::none || board[i][j].color == color_ || pos == Coord{i, j})
                    continue;
                if (board[i][j].piece->isLegalMove(board, pos))
                    return true;
            }
        return false;
    }

    void King::update(const TypePiece board[8][8]) {
        possibleMoves_.clear();
        for (auto &move: legalMoves_) {
            auto pos = Coord{pos_.x + move.x, pos_.y + move.y};
            if (isLegalMove(board, pos)) {
                TypePiece boardCopy[8][8];
                copyBoard(board, boardCopy, pos, pos_);
                if (isCheck(boardCopy, pos))
                    continue;
                possibleMoves_.emplace_back(pos);
            }
        }
    }

    TypePiece King::getType() {
        return {color_, Type::king, this};
    }
}