#ifndef PLAYER
#define PLAYER

#include "logic/Queen.h"
#include "logic/Pawn.h"
#include "logic/Knight.h"
#include "logic/King.h"


namespace chess {

    enum class State {
        normal,
        check,
        checkmate,
        stalemate
    };


    class Player {
    public:
        explicit Player(Color color = Color::white);

        Player(Color color, TypePiece board[8][8]);

        void update(const TypePiece board[8][8]);

        void addPiece(Type type, const Coord& pos);

        Coord updateBoard(TypePiece board[8][8]);

        Piece* getPiece(const Coord& pos);

        [[nodiscard]] State getState() const;

        [[nodiscard]] Color getColor() const { return player_color; }

        [[nodiscard]] Coord getKingPos() const { return king_pos; }

    private:
        std::vector<std::shared_ptr<Piece>> pieces;
        size_t nb_move;
        bool chess;
        Color player_color;
        Coord king_pos;
    };

}

#endif // !PLAYER


