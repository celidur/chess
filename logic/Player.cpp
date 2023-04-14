#include "Player.h"

namespace chess {

    Player::Player(Color color) : isCheck_(false), nbMove_(0), playerColor_(color) {
        int pawnLine = color == Color::white ? 1 : 6;
        int kingLine = color == Color::white ? 0 : 7;
        kingPos_ = {3, kingLine};
        for (int i = 0; i < 16; i++) {
            if (i < 8)
                addPiece(Type::pawn, {i, pawnLine});
            else if (i == 8 or i == 15)
                addPiece(Type::rook, {i % 8, kingLine});
            else if (i == 9 or i == 14)
                addPiece(Type::knight, {i % 8, kingLine});
            else if (i == 10 or i == 13)
                addPiece(Type::bishop, {i % 8, kingLine});
            else if (i == 11)
                addPiece(Type::queen, {4, kingLine});
            else
                addPiece(Type::king, {3, kingLine});
        }
    }


    Player::Player(Color color, TypePiece board[8][8]) : isCheck_(false), nbMove_(0), playerColor_(color),
                                                         kingPos_({-1, -1}) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j].color == color) {
                    addPiece(board[i][j].type, Coord{i, j});
                }
            }
        }
    }

    State Player::getState() const {
        if (nbMove_ == 0 and isCheck_)
            return State::checkmate;
        else if (nbMove_ == 0)
            return State::stalemate;
        else if (isCheck_)
            return State::check;
        return State::normal;
    }

    void Player::update(const TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]) {
        nbMove_ = 0;
        for (auto &&piece: pieces_) {
            piece->update(board, pieceBoard);
            nbMove_ += piece->nbMove();
            auto king = dynamic_cast<King *>(piece.get());
            if (king != nullptr) {
                isCheck_ = king->isCheck(board, nullptr, Coord());
                kingPos_ = king->getPos();
            }
        }
    }

    Coord Player::updateBoard(TypePiece board[8][8], std::shared_ptr<Piece> pieceBoard[8][8]) {
        bool promote = false;
        Coord pos = {-1, -1};
        for (auto it = pieces_.begin(); it != pieces_.end();) {
            auto posPiece = (*it)->getPos();
            if ((*it)->getType().type == Type::pawn && (posPiece.y == 0 || posPiece.y == 7)) {
                pos = posPiece;
                board[posPiece.x][posPiece.y] = (*it)->getType();
                pieceBoard[posPiece.x][posPiece.y] = *it;
                pieces_.erase(it);
            } else if (!(*it)->isAlive()) {
                pieces_.erase(it);
            } else {
                board[posPiece.x][posPiece.y] = (*it)->getType();
                pieceBoard[posPiece.x][posPiece.y] = *it;
                ++it;
            }
        }
        return pos;
    }

    void Player::addPiece(Type type, const Coord &pos) {
        switch (type) {
            case Type::pawn:
                pieces_.push_back(std::make_shared<Pawn>(pos, playerColor_));
                break;
            case Type::rook:
                pieces_.push_back(std::make_shared<Rook>(pos, playerColor_));
                break;
            case Type::knight:
                pieces_.push_back(std::make_shared<Knight>(pos, playerColor_));
                break;
            case Type::bishop:
                pieces_.push_back(std::make_shared<Bishop>(pos, playerColor_));
                break;
            case Type::queen:
                pieces_.push_back(std::make_shared<Queen>(pos, playerColor_));
                break;
            case Type::king:
                pieces_.push_back(std::make_shared<King>(pos, playerColor_));
                kingPos_ = pos;
                break;
            case Type::none:
                break;
        }
    }


}