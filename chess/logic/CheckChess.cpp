/**
* \file   Bishop.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "CheckChess.h"

namespace logic {
    CheckChess::CheckChess(TypePiece (** board)[yBoard], Color color, Coord kingPos,
                           std::vector<std::shared_ptr<Piece>>& pieces, Coord oldPos, Coord newPos)
            : oldPos_(oldPos), newPos_(newPos), kingPos_(kingPos), board_(board),
              color_(color), pieces_(pieces) {
        // swap the piece
        piece_ = (*board_)[newPos_.x][newPos_.y];
        (*board_)[newPos_.x][newPos_.y] = (*board_)[oldPos_.x][oldPos_.y];
        (*board_)[oldPos_.x][oldPos_.y] = {};
    }

    bool CheckChess::isCheck() {
        for (auto&& piece: pieces_) {
            auto pos = piece->getPos();
            if (piece->isLegalMove(*board_, kingPos_) && (*board_)[pos.x][pos.y].color != color_)
                return true;
        }
        return false;
    }

    CheckChess::~CheckChess() {
        // swap the piece
        (*board_)[oldPos_.x][oldPos_.y] = (*board_)[newPos_.x][newPos_.y];
        (*board_)[newPos_.x][newPos_.y] = piece_;
    }

}
