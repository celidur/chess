/**
* \file   Piece.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
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

        virtual bool move(const TypePiece board[8][8], const Coord& pos);

        virtual void update(const TypePiece board[8][8]);

        virtual bool isLegalMove(const TypePiece board[8][8], Coord pos) = 0;

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
