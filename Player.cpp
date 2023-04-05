#include "Player.h"
#include <iostream>

Player::Player(Colour color) : chess(false), nb_move(0), player_color(color) {
    int pawnLine = color == Colour::white ? 1 : 6;
    int kingLine = color == Colour::white ? 0 : 7;
    for (int i = 0; i < 16; i++) {
        if (i < 8)
            pieces.push_back(std::make_unique<Pawn>(Coord{i, pawnLine}, player_color));
        else if (i == 8 or i == 15)
            pieces.push_back(std::make_unique<Tower>(Coord{i % 8, kingLine}, player_color));
        else if (i == 9 or i == 14)
            pieces.push_back(std::make_unique<Knight>(Coord{i % 8, kingLine}, player_color));
        else if (i == 10 or i == 13)
            pieces.push_back(std::make_unique<Bishop>(Coord{i % 8, kingLine}, player_color));
        else if (i == 11)
            pieces.push_back(std::make_unique<Queen>(Coord{4, kingLine}, player_color));
        else
            pieces.push_back(std::make_unique<King>(Coord{3, kingLine}, player_color));
    }
}

State Player::State() const {
    if (nb_move == 0 and chess)
        return State::checkmate;
    else if (nb_move == 0)
        return State::stalemate;
    else if (chess)
        return State::check;
    return State::normal;
}

void Player::update(const TypePiece board[8][8]) {
    nb_move = 0;
    for (auto &&piece: pieces) {
        piece->update(board);
        nb_move += piece->nbMove();
        auto king = dynamic_cast<King *>(piece.get());
        if (king != nullptr) {
            chess = king->isCheck(board);
            king_pos = king->getPos();
        }
    }
}

void Player::updateBoard(TypePiece board[8][8]) {
    for (auto it = pieces.begin(); it != pieces.end();) {
        if (!(*it)->isAlive()) {
            pieces.erase(it);
        } else {
            auto pos = (*it)->getPos();
            board[pos.x][pos.y] = (*it)->getType();
            ++it;
        }
    }
}
