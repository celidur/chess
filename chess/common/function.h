/**
* \file   function.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_FUNCTION_H
#define CHESS_FUNCTION_H

#include "struct.h"

int getPieceValue(TypePiece piece);

int getPieceValue(std::vector<TypePiece>& pieces);

void printBoard(const std::array<std::array<TypePiece, xBoard>, yBoard>& board);

#endif //CHESS_FUNCTION_H
