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
#include "array"
#include "common/function.h"

namespace logic {

    class Piece {
    public:
        Piece(const Coord& pos, TypePiece type);

        static void reset();

        static void resetWhiteKing();

        static void resetBlackKing();

        virtual bool move(const std::array<std::array<TypePiece, xBoard>, yBoard>&, const Coord& pos);

        virtual void update(const std::array<std::array<TypePiece, xBoard>, yBoard>& board);

        virtual bool isLegalMove(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos);

        [[nodiscard]] TypePiece getType();

        [[nodiscard]] Coord getPos() const;

        std::vector<Coord>& getPossibleMoves() ;

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
