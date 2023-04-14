#pragma once
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <ostream>
#include <memory>
#include "common/struct.h"

namespace chess{

    class Piece {
    public:
        Piece(const Coord& pos, Color color);

        static void reset();

        virtual bool move(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], const Coord &pos);

        virtual void update(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]);

        virtual bool isLegalMove(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8], Coord pos) = 0;

        [[nodiscard]] virtual TypePiece getType() = 0;

        [[nodiscard]] Color getColor() const { return color_; }

        [[nodiscard]] bool isAlive() const { return isAlive_; }

        void kill() { isAlive_ = false; }

        [[nodiscard]] size_t nbMove() const { return possibleMoves_.size(); }

        [[nodiscard]] Coord getPos() const { return pos_; }

        [[nodiscard]] std::vector<Coord> getPossibleMoves() const { return possibleMoves_; }

    protected:
        inline static std::shared_ptr<Piece> whiteKing{};
        inline static std::shared_ptr<Piece> blackKing{};
        std::vector<Coord> legalMoves_;
        std::vector<Coord> possibleMoves_;
        Coord pos_;
        Color color_;
        bool isAlive_ = true;
    };
}


#endif // PIECE_H
