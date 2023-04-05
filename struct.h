//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_STRUCT_H
#define CHESS_STRUCT_H

#include <iostream>


struct Coord {
    int x, y;

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

enum class Colour {
    black = 0,
    white = 1,
    none = 2
};

enum class Type {
    pawn,
    tower,
    knight,
    bishop,
    queen,
    king,
    none
};



#endif //CHESS_STRUCT_H
