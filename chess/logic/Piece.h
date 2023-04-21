/**
* \file   Piece.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#pragma once
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <ostream>
#include <memory>
#include "common/function.h"

namespace logic {

    class Piece {
    public:
        Piece(const Coord& pos, Color color);

        static void reset();

        virtual bool move(const TypePiece board[xBoard][yBoard], const Coord& pos);

        virtual void update(const TypePiece board[xBoard][yBoard]);

        virtual bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos);

        [[nodiscard]] virtual TypePiece getType() = 0;

        [[nodiscard]] Coord getPos() const;

        [[nodiscard]] std::vector<Coord> getPossibleMoves() const;

        void setMove(std::vector<Coord>& move);

        void setPromotion();

        bool getPromotion() const;

    protected:
        inline static Piece* whiteKing{};
        inline static Piece* blackKing{};
        std::vector<Coord> legalMoves_;
        std::vector<Coord> possibleMoves_;
        Coord pos_;
        Color color_;
        bool isAlive_;
        bool promotion_;
    };
}


#endif // PIECE_H
