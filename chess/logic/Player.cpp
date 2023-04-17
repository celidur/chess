/**
* \file   Player.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include "Player.h"

namespace logic {

    Player::Player(Color color) : isCheck_(false), nbMove_(0), playerColor_(color) {
        int pawnLine = color == Color::white ? 1 : 6;
        int kingLine = color == Color::white ? 0 : 7;
        kingPos_ = {3, kingLine};
        for (int i = 0; i < 16; i++) {
            if (i < 8)
                pieces_.push_back(std::make_unique<Pawn>(Coord{i, pawnLine}, playerColor_));
            else if (i == 8 or i == 15)
                pieces_.push_back(std::make_unique<Rook>(Coord{i % 8, kingLine}, playerColor_));
            else if (i == 9 or i == 14)
                pieces_.push_back(std::make_unique<Knight>(Coord{i % 8, kingLine}, playerColor_));
            else if (i == 10 or i == 13)
                pieces_.push_back(std::make_unique<Bishop>(Coord{i % 8, kingLine}, playerColor_));
            else if (i == 11)
                pieces_.push_back(std::make_unique<Queen>(Coord{4, kingLine}, playerColor_));
            else
                pieces_.push_back(std::make_unique<King>(Coord{3, kingLine}, playerColor_));
        }
    }


    Player::Player(Color color, TypePiece board[8][8]) : isCheck_(false), nbMove_(0), playerColor_(color),
                                                         kingPos_({-1, -1}) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                if (board[i][j].color == color)
                    addPiece(board[i][j].type, Coord{i, j});
    }

    State Player::getState() const {
        if (isCheck_ && nbMove_ == 0)
            return State::checkmate;
        else if (nbMove_ == 0)
            return State::stalemate;
        else if (isCheck_)
            return State::check;
        return State::normal;
    }

    void Player::update(const TypePiece board[8][8], Player &opponent) {
        nbMove_ = 0;
        for (auto &&piece: pieces_) {
            piece->update(board);
            auto move = piece->getPossibleMoves();
            std::vector<Coord> checkMove;
            for (auto &&m: move) {
                // verify if the move is valid
                TypePiece tmpBoard[8][8];
                copyBoard(board, tmpBoard);
                tmpBoard[piece->getPos().x][piece->getPos().y] = {};
                tmpBoard[m.x][m.y] = piece->getType();
                Coord kingPos = piece->getType().type == Type::king ? m : kingPos_;
                if (!isCheck(tmpBoard, kingPos, opponent)) {
                    checkMove.push_back(m);
                }
            }
            nbMove_ += checkMove.size();
            piece->setMove(checkMove);
            auto king = dynamic_cast<King *>(piece.get());
            if (king != nullptr) {
                isCheck_ = isCheck(board, king->getPos(), opponent);
                kingPos_ = king->getPos();
            }
        }
    }

    Coord Player::updateBoard(TypePiece board[8][8]) {
        Coord pos;
        for (auto it = pieces_.begin(); it != pieces_.end();++it) {
            auto posPiece = (*it)->getPos();
            board[posPiece.x][posPiece.y] = (*it)->getType();
            if ((*it)->getType().type == Type::pawn && (posPiece.y == 0 || posPiece.y == 7)) {
                pos = posPiece;
                pieces_.erase(it);
                --it;
            }
        }
        return pos;
    }

    void Player::addPiece(Type type, const Coord &pos) {
        switch (type) {
            case Type::pawn:
                pieces_.push_back(std::make_unique<Pawn>(pos, playerColor_));
                break;
            case Type::rook:
                pieces_.push_back(std::make_unique<Rook>(pos, playerColor_));
                break;
            case Type::knight:
                pieces_.push_back(std::make_unique<Knight>(pos, playerColor_));
                break;
            case Type::bishop:
                pieces_.push_back(std::make_unique<Bishop>(pos, playerColor_));
                break;
            case Type::queen:
                pieces_.push_back(std::make_unique<Queen>(pos, playerColor_));
                break;
            case Type::king:
                pieces_.push_back(std::make_unique<King>(pos, playerColor_));
                kingPos_ = pos;
                break;
            case Type::none:
                break;
        }

    }

    bool Player::isCheck(const TypePiece board[8][8], Coord kingPos, Player &opponent) {
        // TODO: verify some move can be done to avoid check
        for (auto &&piece: opponent.pieces_) {
            auto pos = piece->getPos();
            if (piece->isLegalMove(board, kingPos) && board[pos.x][pos.y].color != playerColor_)
                return true;
        }
        return false;
    }

    bool Player::move(const TypePiece (*board)[8], const Coord &pos, const Coord &newPos) {
        for (auto &&piece: pieces_) {
            if (piece->getPos() == pos) {
                return piece->move(board, newPos);
            }
        }
        return false;
    }

    void Player::removePiece(const Coord &pos) {
        for (auto &&piece: pieces_) {
            if (piece->getPos() == pos) {
                deadPieces_.push_back(piece->getType());
                pieces_.erase(std::remove(pieces_.begin(), pieces_.end(), piece), pieces_.end());
                return;
            }
        }
    }

    std::vector<Coord> Player::getPossibleMoves(const Coord &pos) {
        for (auto &&piece: pieces_) {
            if (piece->getPos() == pos) {
                return piece->getPossibleMoves();
            }
        }
        return {};
    }

    Coord Player::getKingPos() const { return kingPos_; }

    std::vector<TypePiece> Player::getDeadPieces() const {
        return deadPieces_;
    }


}