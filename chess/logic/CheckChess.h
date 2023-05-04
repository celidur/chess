/**
* \file   Bishop.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_CHECKCHESS_H
#define CHESS_CHECKCHESS_H

#include "common/function.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"
#include "King.h"


namespace logic {
    class CheckChess {
    public:
        CheckChess(TypePiece (** board)[yBoard], const Color& color, const Coord& kingPos,
                   const std::vector<std::shared_ptr<Piece>>& pieces, const Coord& oldPos = Coord{-1, -1},
                   const Coord& newPos = Coord{-1, -1});

        ~CheckChess();

        bool isCheck();

    private:
        Coord oldPos_;
        Coord newPos_;
        const Coord kingPos_;
        TypePiece(** board_)[yBoard];
        const Color color_;
        TypePiece piece_;
        const std::vector<std::shared_ptr<Piece>>& pieces_;
    };

}

#endif //CHESS_CHECKCHESS_H
