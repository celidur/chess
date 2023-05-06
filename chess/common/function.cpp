/**
* \file   function.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "function.h"

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

void printBoard(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) {
    for (int i = 0; i < yBoard; ++i) {
        for (int j = 0; j < xBoard; ++j) {
            std::cout << board[j][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
