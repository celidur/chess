/**
* \file   struct.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_STRUCT_H
#define CHESS_STRUCT_H

#include <iostream>
#include <vector>

constexpr int xBoard = 8;
constexpr int yBoard = 8;
constexpr int maxBoard = xBoard > yBoard ? xBoard : yBoard;

struct Coord {
    int x = -1, y = -1;

    Coord& operator=(const Coord& b) = default;

    bool operator==(const Coord& b) const {
        return x == b.x && y == b.y;
    }

    bool operator!=(const Coord& b) const {
        return x != b.x || y != b.y;
    }

    bool operator<(const int b) const {
        return x < b || y < b;
    }

    bool operator>(const int b) const {
        return x > b || y > b;
    }

    bool operator>=(const Coord& b) const {
        return x >= b.x || y >= b.y;
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

struct CoordF {
    float x, y;
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

    friend std::ostream &operator<<(std::ostream &os, const TypePiece &typePiece){
        switch (typePiece.type) {
            case Type::pawn:
                os << (typePiece.color == Color::white ? "P" : "p");
                break;
            case Type::rook:
                os << (typePiece.color == Color::white ? "R" : "r");
                break;
            case Type::knight:
                os << (typePiece.color == Color::white ? "N" : "n");
                break;
            case Type::bishop:
                os << (typePiece.color == Color::white ? "B" : "b");
                break;
            case Type::queen:
                os << (typePiece.color == Color::white ? "Q" : "q");
                break;
            case Type::king:
                os << (typePiece.color == Color::white ? "K" : "k");
                break;
            default:
                os << " ";
                break;
        }
        return os;
    }
};

#endif //CHESS_STRUCT_H
