#ifndef PLAYER
#define PLAYER

#include "Piece.h"

namespace chess {

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

        void update(const TypePiece board[8][8]);

        void addPiece(Type type, const Coord& pos);

        Coord updateBoard(TypePiece board[8][8]);

        [[nodiscard]] State getState() const;

        [[nodiscard]] Colour getColor() const { return player_color; }

        [[nodiscard]] Coord getKingPos() const { return king_pos; }

    private:
        std::vector<std::shared_ptr<Piece>> pieces;
        size_t nb_move;
        bool chess;
        Colour player_color;
        Coord king_pos;
    };

}

#endif // !PLAYER


