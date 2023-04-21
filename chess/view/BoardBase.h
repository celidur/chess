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
                Coord selection[4],
                TypePiece boardGame[xBoard][yBoard],
                std::vector<Coord>& piecePossibleMove,
                Color color, std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void updatePersonalization(TypePiece boardGame[xBoard][yBoard]) = 0;

        virtual void viewBoard(Color color) = 0;

        virtual void updateDeadPieces(std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void movePiece(Coord& older, Coord& newer) = 0;

        virtual void selectPiece(Coord& pos, std::vector<Coord>& piecePossibleMove) = 0;

        virtual void updatePersonalizationMenu() = 0;

        virtual void updatePiece() = 0;

        virtual void addPiece(TypePiece& typePiece, Coord& pos) = 0;

        virtual void killPiece(Coord& pos) = 0;

        virtual void updateCheckState(Coord& pos) = 0;

        virtual void updatePanel(std::vector<TypePiece> deadPieces[2], int point) = 0;

        virtual void promote(Color color) = 0;
    };
}

#endif //CHESS_BOARDBASE_H
