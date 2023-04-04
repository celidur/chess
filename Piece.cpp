#include "Piece.h"

Piece::Piece(const Coord &pos, Colour color) : pos_(pos), color_(color) {}

bool Piece::move(const std::vector<Piece *> &board, const Coord &pos) {
    if (isLegalMove(board, pos)) {
        pos_ = pos;
        return true;
    }
    return false;
}

Tower::Tower(const Coord &pos, Colour color) : Piece(pos, color) {}

bool Tower::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    Piece *piece = board[pos.y * 8 + pos.x];
    if (piece != nullptr && piece->getColor() == color_) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y) {
        return false;
    }
    if (pos.x != pos_.x && pos.y != pos_.y) {
        return false;
    }
    int x = pos.x - pos_.x;
    int y = pos.y - pos_.y;
    int i = 1;
    while (i < abs(x) + abs(y)) {
        if (x != 0) {
            if (board[pos_.y * 8 + pos_.x + i * x / abs(x)] != nullptr) {
                return false;
            }
        } else {
            if (board[(pos_.y + i * y / abs(y)) * 8 + pos_.x] != nullptr) {
                return false;
            }
        }
        i++;
    }
    return true;
}

bool Tower::getFirst() const {
    return first_;
}

bool Tower::move(const std::vector<Piece *> &board, const Coord &pos) {
    if (Piece::move(board, pos)) {
        first_ = false;
        return true;
    }
    return false;
}

Bishop::Bishop(const Coord &pos, Colour color) : Piece(pos, color) {}

bool Bishop::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    Piece *piece = board[pos.y * 8 + pos.x];
    if (piece != nullptr && piece->getColor() == color_) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y) {
        return false;
    }
    if (abs(pos.x - pos_.x) != abs(pos.y - pos_.y)) {
        return false;
    }
    int x = pos.x - pos_.x;
    int y = pos.y - pos_.y;
    int i = 1;
    while (i < abs(x)) {
        if (board[(pos_.y + i * y / abs(y)) * 8 + pos_.x + i * x / abs(x)] != nullptr) {
            return false;
        }
        i++;
    }
    return true;
}

Queen::Queen(const Coord &pos, Colour color) : Piece(pos, color), Tower(pos, color), Bishop(pos, color) {}

bool Queen::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    return Tower::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
}

King::King(const Coord &pos, Colour color) : Piece(pos, color) {
    if (color == Colour::white) {
        if (whiteKingPos != nullptr)
            throw std::runtime_error("White king already exists");
        whiteKingPos = &pos_;
    } else {
        if (blackKingPos != nullptr)
            throw std::runtime_error("Black king already exists");
        blackKingPos = &pos_;
    }
}

bool King::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    Piece *piece = board[pos.y * 8 + pos.x];
    if (piece != nullptr && piece->getColor() == color_) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y) {
        return false;
    }
    if (pos.x - pos_.x > 1 || pos.x - pos_.x < -1 || pos.y - pos_.y > 1 || pos.y - pos_.y < -1) {
        return false;
    }
    if ((piece != nullptr || board[pos.y * 8 + pos.x + 1] == nullptr || first_) && pos.x == pos_.x + 2 &&
        pos.y == pos_.y) {
        std::vector<Piece *> boardCopy = board;
        boardCopy[pos_.y * 8 + pos_.x + 1] = boardCopy[pos_.y * 8 + pos_.x];
        boardCopy[pos_.y * 8 + pos_.x] = nullptr;
        if (isCheck(boardCopy)) {
            return false;
        }
        auto tower = dynamic_cast<Tower *>(board[pos.y * 8 + pos.x + 3]);
        if (tower == nullptr)
            return false;
        if (tower->getColor() != color_ || tower->getFirst())
            return false;
    }
    if ((piece != nullptr || board[pos.y * 8 + pos.x - 1] == nullptr || first_) && pos.x == pos_.x - 2 &&
        pos.y == pos_.y) {
        auto tower = dynamic_cast<Tower *>(board[pos.y * 8 + pos.x - 4]);
        if (tower == nullptr)
            return false;
        if (tower->getColor() != color_ || tower->getFirst())
            return false;
    }
    return true;

}

bool King::move(const std::vector<Piece *> &board, const Coord &pos) {
    if (isLegalMove(board, pos)) {
        if (pos.x == pos_.x + 2 && pos.y == pos_.y) {
            auto tower = dynamic_cast<Tower *>(board[pos.y * 8 + pos.x + 3]);
            tower->move(board, {pos.x - 1, pos.y});
        }
        if (pos.x == pos_.x - 2 && pos.y == pos_.y) {
            auto tower = dynamic_cast<Tower *>(board[pos.y * 8 + pos.x - 4]);
            tower->move(board, {pos.x + 1, pos.y});
        }
        pos_ = pos;
        first_ = false;
        return true;
    }
    return false;
}

bool King::isCheck(const std::vector<Piece *> &board) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (board[i * 8 + j] == nullptr)
                continue;
            if (board[i * 8 + j]->isLegalMove(board, pos_) && board[i * 8 + j]->getColor() != color_)
                return true;
        }
    return false;
}

Pawn::Pawn(const Coord &pos, Colour color) : Piece(pos, color) {}

bool Pawn::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    int direction = color_ == Colour::white ? 1 : -1;
    Piece *piece = board[pos.y * 8 + pos.x];
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y + direction) {
        return piece == nullptr;
    }
    if (pos.x == pos_.x && pos.y == pos_.y + 2 * direction && first_ == 0) {
        return piece == nullptr && board[(pos.y - direction) * 8 + pos.x] == nullptr;
    }
    if ((pos.x == pos_.x + 1 || pos.x == pos_.x - 1) && pos.y == pos_.y + direction) {
        if (piece != nullptr)
            return piece->getColor() != color_; // for eating
        auto pawn = dynamic_cast<Pawn *>(board[pos_.y * 8 + pos.x]); // en passant
        if (pawn == nullptr)
            return false;
        if (pawn->color_ != color_ && pawn->first_ == 1)
            return true;
    }
    return false;
}

bool Pawn::move(const std::vector<Piece *> &board, const Coord &pos) {
    if (Piece::move(board, pos)) {
        if (pos.x == pos_.x + 1 || pos.x == pos_.x - 1) {
            auto pawn = dynamic_cast<Pawn *>(board[pos_.y * 8 + pos.x]);
            if (pawn != nullptr && board[pos.y * 8 + pos.x] == nullptr) {
                if (pawn->first_ == 1)
                    pawn->isAlive_ = false;
            }
        }
        if (pos.x == pos_.x +2 || pos.x == pos_.x - 2)
            first_ = 1;
        first_ = (first_ == 1 | first_ == -1) ? -1 : 0;
        return true;
    }
    return false;
}


Knight::Knight(const Coord &pos, Colour color) : Piece(pos, color) {}

bool Knight::isLegalMove(const std::vector<Piece *> &board, Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    Piece *piece = board[pos.y * 8 + pos.x];
    if (piece != nullptr && piece->getColor() == color_) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y) {
        return false;
    }
    if (abs(pos.x - pos_.x) == 2 && abs(pos.y - pos_.y) == 1) {
        return true;
    }
    if (abs(pos.x - pos_.x) == 1 && abs(pos.y - pos_.y) == 2) {
        return true;
    }
    return false;
}
