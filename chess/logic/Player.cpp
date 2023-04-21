/**
* \file   Player.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Player.h"

namespace logic {

    Player::Player(Color color) : isCheck_(false), nbMove_(0), playerColor_(color) {
    }


    Player::Player(Color color, TypePiece board[xBoard][yBoard]) : isCheck_(false), nbMove_(0), playerColor_(color),
                                                         kingPos_({-1, -1}) {
        for (int i = 0; i < xBoard; ++i)
            for (int j = 0; j < yBoard; ++j)
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

    void Player::update(const TypePiece board[xBoard][yBoard], Player& opponent) {
        nbMove_ = 0;
        for (auto&& piece: pieces_) {
            piece->update(board);
            auto moves = piece->getPossibleMoves();
            std::vector<Coord> checkMove;
            for (auto&& move: moves) {
                // verify if the move is valid
                TypePiece tmpBoard[xBoard][yBoard];
                copyBoard(board, tmpBoard, move, piece->getPos());
                Coord kingPos = piece->getType().type == Type::king ? move : kingPos_;
                if (!isCheck(tmpBoard, kingPos, opponent)) {
                    if (kingPos == move && abs(piece->getPos().x - move.x) == 2) {
                        auto m = piece->getPos().x < move.x ? Coord{kingPos_.x + 1, kingPos.y} : Coord{kingPos_.x - 1,
                                                                                                       kingPos.y};
                        copyBoard(board, tmpBoard, m, piece->getPos());
                        if (isCheck(tmpBoard, m, opponent))
                            continue;
                    }
                    checkMove.push_back(move);
                }
            }
            nbMove_ += checkMove.size();
            piece->setMove(checkMove);
            auto king = dynamic_cast<King*>(piece.get());
            if (king != nullptr) {
                isCheck_ = isCheck(board, king->getPos(), opponent);
                kingPos_ = king->getPos();
            }
        }
    }

    Coord Player::updateBoard(TypePiece board[xBoard][yBoard]) {
        Coord pos;
        for (auto it = pieces_.begin(); it != pieces_.end(); ++it) {
            auto posPiece = (*it)->getPos();
            board[posPiece.x][posPiece.y] = (*it)->getType();
            if ((*it)->getType().type == Type::pawn && (posPiece.y == 0 || posPiece.y == yBoard - 1)) {
                pos = posPiece;
                pieces_.erase(it);
                --it;
            }
        }
        return pos;
    }

    void Player::addPiece(Type type, const Coord& pos, bool isPromotion) {
        switch (type) {
            case Type::pawn:
                pieces_.emplace_back(std::make_unique<Pawn>(pos, playerColor_));
                break;
            case Type::rook:
                pieces_.emplace_back(std::make_unique<Rook>(pos, playerColor_));
                break;
            case Type::knight:
                pieces_.emplace_back(std::make_unique<Knight>(pos, playerColor_));
                break;
            case Type::bishop:
                pieces_.emplace_back(std::make_unique<Bishop>(pos, playerColor_));
                break;
            case Type::queen:
                pieces_.emplace_back(std::make_unique<Queen>(pos, playerColor_));
                break;
            case Type::king:
                pieces_.emplace_back(std::make_unique<King>(pos, playerColor_));
                kingPos_ = pos;
                break;
            case Type::none:
                return;
        }
        if (isPromotion)
            pieces_[pieces_.size() - 1]->setPromotion();
    }

    bool Player::isCheck(const TypePiece board[xBoard][yBoard], Coord kingPos, Player& opponent) {
        for (auto&& piece: opponent.pieces_) {
            auto pos = piece->getPos();
            if (piece->isLegalMove(board, kingPos) && board[pos.x][pos.y].color != playerColor_)
                return true;
        }
        return false;
    }

    bool Player::move(const TypePiece board[xBoard][yBoard], const Coord& pos, const Coord& newPos) {
        for (auto&& piece: pieces_) {
            if (piece->getPos() == pos) {
                return piece->move(board, newPos);
            }
        }
        return false;
    }

    void Player::removePiece(const Coord& pos) {
        for (auto&& piece: pieces_) {
            if (piece->getPos() == pos) {
                TypePiece type = piece->getPromotion() ? TypePiece{playerColor_, Type::pawn} : piece->getType();
                deadPieces_.push_back(type);
                pieces_.erase(std::remove(pieces_.begin(), pieces_.end(), piece), pieces_.end());
                return;
            }
        }
    }

    std::vector<Coord> Player::getPossibleMoves(const Coord& pos) {
        for (auto&& piece: pieces_) {
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