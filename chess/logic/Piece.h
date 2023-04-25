/**
* \file   Piece.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <ostream>
#include <memory>
#include "common/function.h"

namespace logic {

    class Piece {
    public:
        Piece(const Coord& pos, const TypePiece type);

        static void reset();

        virtual bool move(const TypePiece board[xBoard][yBoard], const Coord& pos);

        virtual void update(const TypePiece board[xBoard][yBoard]);

        virtual bool isLegalMove(const TypePiece board[xBoard][yBoard], Coord pos);

        [[nodiscard]] TypePiece getType();

        [[nodiscard]] Coord getPos() const;

        [[nodiscard]] std::vector<Coord> getPossibleMoves() const;

        void setMove(std::vector<Coord>& move);

        void setPromotion();

        [[nodiscard]] bool getPromotion() const;

    protected:
        inline static Piece* whiteKing{};
        inline static Piece* blackKing{};
        std::vector<Coord> legalMoves_;
        std::vector<Coord> possibleMoves_;
        Coord pos_;
        TypePiece type_;
        bool promotion_;
    };
}


#endif // PIECE_H
