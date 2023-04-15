//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_STRUCT_H
#define CHESS_STRUCT_H

#include <vector>


struct Coord {
    int x = -1, y = -1;

    Coord &operator=(const Coord &b) = default;

    bool operator==(const Coord &b) const {
        return x == b.x && y == b.y;
    }

    bool operator!=(const Coord &b) const {
        return x != b.x || y != b.y;
    }

    Coord operator+(const Coord &b) const {
        return Coord{x + b.x, y + b.y};
    }

    Coord operator-(const Coord &b) const {
        return Coord{x - b.x, y - b.y};
    }

    friend std::ostream &operator<<(std::ostream &os, const Coord &coord) {
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
    personalisation
};

struct TypePiece {
    Color color = Color::none;
    Type type = Type::none;
    int first = 0;
};

namespace screen {


    class BoardBase {
    public:
        virtual ~BoardBase() = default;

        virtual void updateGame(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color) = 0;

        virtual void updatePersonnalisation(TypePiece boardGame[8][8]) = 0;

        virtual void viewBoard(Color color) = 0;
    };
}


#endif //CHESS_STRUCT_H
