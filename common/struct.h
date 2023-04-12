//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_STRUCT_H
#define CHESS_STRUCT_H

#include <iostream>
#include <vector>


struct Coord {
    int x = -1, y = -1;

    Coord& operator=(const Coord& b) = default;

    bool operator==(const Coord& b) const {
        return x == b.x && y == b.y;
    }

    bool operator!=(const Coord& b) const {
        return x != b.x || y != b.y;
    }

    Coord operator+(const Coord& b) const {
        return Coord{x + b.x, y + b.y};
    }

    Coord operator-(const Coord& b) const {
        return Coord{x - b.x, y - b.y};
    }

    friend std::ostream& operator<<(std::ostream& os, const Coord& coord) {
        return os << coord.x << " " << coord.y;
    }
};

enum class Color {
    black = 0,
    white = 1,
    none = 2
};

enum class Type {
    pawn,
    rook,
    knight,
    bishop,
    queen,
    king,
    none
};

enum class Mode {
    game,
    menu,
    personalisation
};

struct TypePiece {
    Color color = Color::none;
    Type type = Type::none;
    int first = 0;
};

void copyBoard(const TypePiece board[8][8], TypePiece boardCopy[8][8], Coord swap = Coord{-1, -1},
               Coord swap2 = Coord{-1, -1});


void copyBoard(const TypePiece board[8][8], TypePiece boardCopy[8][8], Coord swap, Coord swap2) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boardCopy[i][j] = board[i][j];
        }
    }
    if (swap.x != -1 && swap.y != -1) {
        boardCopy[swap.x][swap.y] = board[swap2.x][swap2.y];
        boardCopy[swap2.x][swap2.y] = {};
    }
}

namespace screen {


    class BoardBase {
    public:
        virtual ~BoardBase() = default;

        virtual void update(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color) = 0;

        virtual void update(TypePiece boardGame[8][8]) = 0;

        virtual void viewBoard(Color color) = 0;
    };
}


#endif //CHESS_STRUCT_H
