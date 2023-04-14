//
// Created by Gaetan on 2023-04-14.
//

#include "fuction.h"

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
