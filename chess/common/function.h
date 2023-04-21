/**
* \file   function.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_FUNCTION_H
#define CHESS_FUNCTION_H

#include "struct.h"

int getPieceValue(TypePiece piece);

int getPieceValue(std::vector<TypePiece>& pieces);

#endif //CHESS_FUNCTION_H
