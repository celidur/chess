#ifndef PLAYER
#define PLAYER

#include "Piece.h"

enum class State {
    normal,
    check,
    checkmate,
    stalemate
};


class Player {
public:
    explicit Player(Colour color = Colour::white);

    void update(const TypePiece board[8][8]);

    void updateBoard(TypePiece board[8][8]);

    [[nodiscard]] State State() const;

    [[nodiscard]] Colour getColor() const { return player_color; }

    [[nodiscard]] Coord getKingPos() const { return king_pos; }

private:
    std::vector<std::shared_ptr<Piece>> pieces;
    size_t nb_move;
    bool chess;
    Colour player_color;
    Coord king_pos;
};

#endif // !PLAYER


