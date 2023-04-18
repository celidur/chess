/**
* \file   function.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "function.h"

void copyBoard(const TypePiece board[8][8], TypePiece boardCopy[8][8], Coord swap,
               Coord swap2) {
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

int getPieceValue(TypePiece piece) {
    switch (piece.type) {
        case Type::pawn:
            return 1;
        case Type::knight:
        case Type::bishop:
            return 3;
        case Type::rook:
            return 5;
        case Type::queen:
            return 9;
        case Type::king:
            return 100;
        default:
            return 0;
    }
}

int getPieceValue(std::vector<TypePiece>& pieces) {
    int value = 0;
    for (auto& piece: pieces) {
        value += getPieceValue(piece);
    }
    return value;
}
