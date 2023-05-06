/**
* \file   Player.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include "Player.h"

namespace logic {

    Player::Player(const Color &color) : isCheck_(false), nbMove_(0), playerColor_(color) {
    }


    Player::Player(const Color &color, const std::array<std::array<TypePiece, xBoard>, yBoard>& board) : isCheck_(false), nbMove_(0),
                                                                                playerColor_(color),
                                                                                kingPos_({-1, -1}) {
        for (int i = 0; i < xBoard; ++i)
            for (int j = 0; j < yBoard; ++j)
                if (board[i][j].color == color) {
                    try {
                        addPiece(board[i][j].type, Coord{i, j});
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
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

    void Player::update(std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Player &opponent) {
        nbMove_ = 0;
        for (auto &&piece: pieces_) {
            piece->update(board);
            auto moves = piece->getPossibleMoves();

            for (auto &&move: moves) {
                Coord kingPos = piece->getType().type == Type::king ? move : kingPos_;
                auto pos = piece->getPos();
                if (CheckChess(board, playerColor_, kingPos, opponent.pieces_, pos, move).isCheck()) {
                    moves.erase(std::find(moves.begin(), moves.end(), move));
                    continue;
                }
                if (kingPos == move && abs(pos.x - move.x) == 2) {
                    auto m = pos.x < move.x ? Coord{kingPos_.x + 1, kingPos.y} : Coord{kingPos_.x - 1, kingPos.y};
                    if (CheckChess(board, playerColor_, m, opponent.pieces_, pos, m).isCheck()) {
                        moves.erase(std::find(moves.begin(), moves.end(), move));
                        continue;
                    }
                }
            }
            nbMove_ += moves.size();
            auto king = dynamic_cast<King *>(piece.get());
            if (king != nullptr) {
                isCheck_ = CheckChess(board, playerColor_, kingPos_, opponent.pieces_).isCheck();
                kingPos_ = king->getPos();
            }
        }
    }

    Coord Player::updateBoard(std::array<std::array<TypePiece, xBoard>, yBoard>& board) {
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

    void Player::addPiece(const Type type, const Coord &pos, bool isPromotion) {
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
                pieces_.erase(std::remove_if(pieces_.begin(), pieces_.end(),
                                             [&](std::shared_ptr<Piece> &p) { return p->getPos() == pos; }),
                              pieces_.end());
        }
        if (isPromotion)
            pieces_[pieces_.size() - 1]->setPromotion();
    }

    bool Player::move(const std::array<std::array<TypePiece, xBoard>, yBoard>& board, const Coord &pos, const Coord &newPos) {
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
                TypePiece type = piece->getPromotion() ? TypePiece{playerColor_, Type::pawn} : piece->getType();
                deadPieces_.push_back(type);
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