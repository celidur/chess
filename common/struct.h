/**
* \file   struct.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_STRUCT_H
#define CHESS_STRUCT_H

#include <iostream>
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

enum class Colour {
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
    Colour color = Colour::none;
    Type type = Type::none;
    int first = 0;
};


#endif //CHESS_STRUCT_H
