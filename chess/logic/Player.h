/**
* \file   Player.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef PLAYER
#define PLAYER

#include "algorithm"
#include "array"
#include "CheckChess.h"


namespace logic {

    enum class State {
        normal,
        check,
        checkmate,
        stalemate
    };


    class Player {
    public:
        explicit Player(const Color& color = Color::white);

        Player(const Color& color, const std::array<std::array<TypePiece, xBoard>, yBoard>& board);

        void update(std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Player& opponent);

        void addPiece(Type type, const Coord& pos, bool isPromotion = false);

        Coord updateBoard(std::array<std::array<TypePiece, xBoard>, yBoard>& board);

        bool
        move(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord& pos, const Coord& newPos);

        void removePiece(const Coord& pos);

        std::vector<Coord> getPossibleMoves(const Coord& pos);

        [[nodiscard]] std::vector<TypePiece> getDeadPieces() const;

        [[nodiscard]] State getState() const;

        [[nodiscard]] Coord getKingPos() const;

    private:

        std::vector<std::shared_ptr<Piece>> pieces_;
        std::vector<TypePiece> deadPieces_;
        size_t nbMove_;
        bool isCheck_;
        Color playerColor_;
        Coord kingPos_;
    };

}

#endif // !PLAYER


