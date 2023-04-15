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

namespace logic{
    class Piece;

    struct TypePiece {
        Colour color = Colour::none;
        Type type = Type::none;
        Piece *piece = nullptr;
        int first = 0;

        friend std::ostream& operator<<(std::ostream& os, const TypePiece& piece) {
            switch (piece.type) {
                case Type::pawn:
                    os << (piece.color == Colour::white ? "p" : "P");
                    break;
                case Type::rook:
                    os << (piece.color == Colour::white ? "t" : "T");
                    break;
                case Type::knight:
                    os << (piece.color == Colour::white ? "c" : "C");
                    break;
                case Type::bishop:
                    os << (piece.color == Colour::white ? "f" : "F");
                    break;
                case Type::queen:
                    os << (piece.color == Colour::white ? "q" : "Q");
                    break;
                case Type::king:
                    os << (piece.color == Colour::white ? "k" : "K");
                    break;
                case Type::none:
                    os << " ";
                    break;
            }
            return os;
        }
    };


    class Piece {
    public:
        Piece(const Coord& pos, Colour color);

        static void reset();

        virtual bool move(const TypePiece board[8][8], const Coord &pos);

        virtual void update(const TypePiece board[8][8]);

        virtual bool isLegalMove(const TypePiece board[8][8], Coord pos) = 0;

        [[nodiscard]] virtual TypePiece getType() = 0;

        [[nodiscard]] Coord getPos() const;

        [[nodiscard]] std::vector<Coord> getPossibleMoves() const;

        void setMove(std::vector<Coord>& move);

        Colour getColor() const { return color_; }

    protected:
        inline static Piece* whiteKing{};
        inline static Piece* blackKing{};
        std::vector<Coord> legalMoves_;
        std::vector<Coord> possibleMoves_;
        Coord pos_;
        Colour color_;
        bool isAlive_;
    };
}


#endif // PIECE_H
