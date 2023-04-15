/**
* \file   Player.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef PLAYER
#define PLAYER

#include "algorithm"
#include "logic/Queen.h"
#include "logic/Pawn.h"
#include "logic/Knight.h"
#include "logic/King.h"


namespace logic {

    enum class State {
        normal,
        check,
        checkmate,
        stalemate
    };


    class Player {
    public:
        explicit Player(Colour color = Colour::white);

        Player(Colour color, TypePiece board[8][8]);

        void update(const TypePiece board[8][8], Player &opponent);

        void addPiece(Type type, const Coord& pos);

        Coord updateBoard(TypePiece board[8][8]);

        bool move(const TypePiece board[8][8], const Coord &pos, const Coord &newPos);

        void removePiece(const Coord &pos);

        std::vector<Coord> getPossibleMoves(const Coord &pos);

        [[nodiscard]] std::vector<TypePiece> getDeadPieces() const;

        [[nodiscard]] State getState() const;

        [[nodiscard]] Coord getKingPos() const;

    private:
        bool isCheck(const TypePiece board[8][8], Coord kingPos, Player &opponent);
        std::vector<std::shared_ptr<Piece>> pieces_;
        std::vector<TypePiece> deadPieces_;
        size_t nbMove_;
        bool isCheck_;
        Colour playerColor_;
        Coord kingPos_;
    };

}

#endif // !PLAYER


