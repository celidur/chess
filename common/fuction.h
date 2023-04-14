//
// Created by Gaetan on 2023-04-14.
//

#ifndef CHESS_FUCTION_H
#define CHESS_FUCTION_H

#include "struct.h"

void copyBoard(const TypePiece board[8][8], TypePiece boardCopy[8][8], Coord swap = Coord{-1, -1},
               Coord swap2 = Coord{-1, -1});

#endif //CHESS_FUCTION_H
