/**
* \file   BoardBase.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_BOARDBASE_H
#define CHESS_BOARDBASE_H

#include "common/function.h"

namespace view {

    class BoardBase {
    public:
        virtual ~BoardBase() = default;

        virtual void updateGame(
                const Coord selection[4],
                const TypePiece boardGame[xBoard][yBoard],
                const std::vector<Coord>& piecePossibleMove,
                const Color& color, const std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void updatePersonalization(const TypePiece boardGame[xBoard][yBoard]) = 0;

        virtual void viewBoard(const Color& color) = 0;

        virtual void updateDeadPieces(const std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void movePiece(Coord& older, Coord& newer) = 0;

        virtual void selectPiece(const Coord& pos, const std::vector<Coord>& piecePossibleMove) = 0;

        virtual void updatePersonalizationMenu() = 0;

        virtual void updatePiece() = 0;

        virtual void addPiece(const TypePiece& typePiece, const Coord& pos) = 0;

        virtual void killPiece(const Coord& pos) = 0;

        virtual void updateCheckState(const Coord& pos) = 0;

        virtual void updatePanel(const std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void promote(const Color& color) = 0;
    };
}

#endif //CHESS_BOARDBASE_H
