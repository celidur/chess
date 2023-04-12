#include "Player.h"
#include <iostream>

namespace chess {

    Player::Player(Color color) : chess(false), nb_move(0), player_color(color) {
        int pawnLine = color == Color::white ? 1 : 6;
        int kingLine = color == Color::white ? 0 : 7;
        king_pos = {3, kingLine};
        for (int i = 0; i < 16; i++) {
            if (i < 8)
                pieces.push_back(std::make_unique<Pawn>(Coord{i, pawnLine}, player_color));
            else if (i == 8 or i == 15)
                pieces.push_back(std::make_unique<Rook>(Coord{i % 8, kingLine}, player_color));
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


    Player::Player(Color color, TypePiece board[8][8]) : chess(false), nb_move(0), player_color(color),
                                                         king_pos({-1, -1}) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j].color == color) {
                    addPiece(board[i][j].type, Coord{i, j});
                }
            }
        }
    }

    State Player::getState() const {
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

    Coord Player::updateBoard(TypePiece board[8][8]) {
        bool promote = false;
        Coord pos = {-1, -1};
        for (auto it = pieces.begin(); it != pieces.end();) {
            auto posPiece = (*it)->getPos();
            if ((*it)->getType().type == Type::pawn && (posPiece.y == 0 || posPiece.y == 7)) {
                pos = posPiece;
                board[posPiece.x][posPiece.y] = (*it)->getType();
                pieces.erase(it);
            } else if (!(*it)->isAlive()) {
                pieces.erase(it);
            } else {
                board[posPiece.x][posPiece.y] = (*it)->getType();
                ++it;
            }
        }
        return pos;
    }

    void Player::addPiece(Type type, const Coord &pos) {
        switch (type) {
            case Type::pawn:
                pieces.push_back(std::make_unique<Pawn>(pos, player_color));
                break;
            case Type::rook:
                pieces.push_back(std::make_unique<Rook>(pos, player_color));
                break;
            case Type::knight:
                pieces.push_back(std::make_unique<Knight>(pos, player_color));
                break;
            case Type::bishop:
                pieces.push_back(std::make_unique<Bishop>(pos, player_color));
                break;
            case Type::queen:
                pieces.push_back(std::make_unique<Queen>(pos, player_color));
                break;
            case Type::king:
                pieces.push_back(std::make_unique<King>(pos, player_color));
                king_pos = pos;
                break;
            case Type::none:
                break;
        }

    }


}