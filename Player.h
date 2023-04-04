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

    void update(TypePiece board[8][8]);

    void updateBoard(TypePiece board[8][8]);

    [[nodiscard]] State State() const;

    [[nodiscard]] Colour getColor() const { return player_color; }

private:
    std::vector<std::shared_ptr<Piece>> pieces;
    size_t nb_move;
    bool chess;
    Colour player_color;
};

#endif // !PLAYER


