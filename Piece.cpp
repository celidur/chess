#include "Piece.h"

Piece::Piece(const Coord &pos, Colour color) : pos_(pos), color_(color) {}

bool Piece::move(const TypePiece board[8][8], const Coord &pos) {
    if (std::find(possibleMoves_.begin(), possibleMoves_.end(), pos) != possibleMoves_.end()) {
        pos_ = pos;
        if (board[pos_.x][pos_.y].type != Type::none) {
            board[pos_.x][pos_.y].piece->kill();
        }
        return true;
    }
    return false;
}

void Piece::update(const TypePiece board[8][8]) {
    possibleMoves_.clear();
    for (const auto &move: legalMoves_) {
        Coord pos = pos_ + move;
        if (isLegalMove(board, pos)) {
            TypePiece boardCopy[8][8];
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    boardCopy[i][j] = board[i][j];
                }
            }
            boardCopy[pos.x][pos.y] = boardCopy[pos_.x][pos_.y];
            boardCopy[pos_.x][pos_.y] = TypePiece{Colour::none, Type::none, nullptr, 0};

            if (color_ == Colour::white) {
                if (!whiteKing->isLegalMove(boardCopy, whiteKing->pos_)) {
                    continue;
                }
            } else {
                if (!blackKing->isLegalMove(boardCopy, blackKing->pos_)) {
                    continue;
                }
            }
            possibleMoves_.push_back(pos);
        }
    }
}

Tower::Tower(const Coord &pos, Colour color) : Piece(pos, color) {
    for (int i = 0; i < 8; i++) {
        legalMoves_.emplace_back(Coord{i, 0});
        legalMoves_.emplace_back(Coord{0, i});
        legalMoves_.emplace_back(Coord{-i, 0});
        legalMoves_.emplace_back(Coord{0, -i});
    }
}

bool Tower::isLegalMove(const TypePiece board[8][8], Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    auto piece = board[pos.x][pos.y];
    if (piece.type != Type::none && piece.color == color_) {
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
            if (board[pos_.x + i * x / abs(x)][pos.y].type != Type::none) {
                return false;
            }
        } else {
            if (board[pos_.x][pos_.y + i * y / abs(y)].type != Type::none) {
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

bool Tower::move(const TypePiece board[8][8], const Coord &pos) {
    bool res = Piece::move(board, pos);
    if (res)
        first_ = false;
    return res;
}

TypePiece Tower::getType() {
    return {color_, Type::tower, this, first_};
}

Bishop::Bishop(const Coord &pos, Colour color) : Piece(pos, color) {
    for (int i = 0; i < 8; i++) {
        legalMoves_.emplace_back(Coord{i, i});
        legalMoves_.emplace_back(Coord{-i, i});
        legalMoves_.emplace_back(Coord{i, -i});
        legalMoves_.emplace_back(Coord{-i, -i});
    }
}

bool Bishop::isLegalMove(const TypePiece board[8][8], Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    auto piece = board[pos.x][pos.y];
    if (piece.type != Type::none && piece.color == color_) {
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
        if (board[pos_.x + i * x / abs(x)][pos_.y + i * y / abs(y)].type != Type::none) {
            return false;
        }
        i++;
    }
    return true;
}

TypePiece Bishop::getType() {
    return {color_, Type::bishop, this};
}

Queen::Queen(const Coord &pos, Colour color) : Piece(pos, color), Tower(pos, color), Bishop(pos, color) {}

bool Queen::isLegalMove(const TypePiece board[8][8], Coord pos) {
    return Tower::isLegalMove(board, pos) || Bishop::isLegalMove(board, pos);
}

TypePiece Queen::getType() {
    return {color_, Type::queen, this};
}

King::King(const Coord &pos, Colour color) : Piece(pos, color) {
    if (color == Colour::white) {
        if (whiteKing != nullptr)
            throw std::runtime_error("White king already exists");
        whiteKing = this;
    } else {
        if (blackKing != nullptr)
            throw std::runtime_error("Black king already exists");
        blackKing = this;
    }
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;
            legalMoves_.emplace_back(Coord{i, j});
        }
    }
    legalMoves_.emplace_back(Coord{2, 0});
    legalMoves_.emplace_back(Coord{-2, 0});
}

bool King::isLegalMove(const TypePiece board[8][8], Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    auto piece = board[pos.x][pos.y];
    if (piece.type != Type::none && piece.color == color_) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y) {
        return false;
    }
    if (pos.x - pos_.x > 1 || pos.x - pos_.x < -1 || pos.y - pos_.y > 1 || pos.y - pos_.y < -1) {
        return false;
    }
    if ((piece.type == Type::none || board[pos.x - 1][pos.y].type == Type::none || first_) && pos.x == pos_.x + 2 &&
        pos.y == pos_.y) {
        TypePiece boardCopy[8][8];
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                boardCopy[i][j] = board[i][j];
        boardCopy[pos_.x + 1][pos_.y] = boardCopy[pos_.x][pos_.y];
        boardCopy[pos_.x][pos_.y] = {Colour::none, Type::none, nullptr};
        if (isCheck(boardCopy, Coord{pos_.x + 1, pos_.y})) {
            return false;
        }
        auto tower = board[pos_.x + 3][pos_.y];
        if (tower.type != Type::tower || tower.color != color_ || tower.first == false)
            return false;
    }
    if ((piece.type == Type::none || board[pos.x + 1][pos.y].type == Type::none || !first_) && pos.x == pos_.x - 2 &&
        pos.y == pos_.y) {
        TypePiece boardCopy[8][8];
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                boardCopy[i][j] = board[i][j];
        boardCopy[pos_.x - 1][pos_.y] = boardCopy[pos_.y][pos_.x];
        boardCopy[pos_.x][pos_.y] = {Colour::none, Type::none, nullptr};
        if (isCheck(boardCopy, Coord{pos_.x - 1, pos_.y})) {
            return false;
        }
        auto tower = board[pos_.x - 4][pos_.y];
        if (tower.type != Type::tower || tower.color != color_ || tower.first == false)
            return false;
    }
    return true;

}

bool King::move(const TypePiece board[8][8], const Coord &pos) {
    bool res = Piece::move(board, pos);
    if (!res)
        return false;
    if (pos.x == pos_.x + 2 && pos.y == pos_.y) {
        auto tower = dynamic_cast<Tower *>(board[pos_.x + 3][pos.y].piece);
        tower->move(board, {pos.x - 1, pos.y});
    }
    if (pos.x == pos_.x - 2 && pos.y == pos_.y) {
        auto tower = dynamic_cast<Tower *>(board[pos_.x - 4][pos.y].piece);
        tower->move(board, {pos.x + 1, pos.y});
    }
    pos_ = pos;
    first_ = false;
    return true;
}

bool King::isCheck(const TypePiece board[8][8], Coord pos) {
    if (pos == Coord{-1, -1})
        pos = pos_;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (board[i][j].type == Type::none || board[i][j].color == color_)
                continue;
            if (board[i][j].piece->isLegalMove(board, pos))
                return true;
        }
    return false;
}

void King::update(const TypePiece board[8][8]) {
    possibleMoves_.clear();
    for (auto &move: legalMoves_) {
        auto pos = Coord{pos_.x + move.x, pos_.y + move.y};
        TypePiece boardCopy[8][8];
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                boardCopy[i][j] = board[i][j];
        boardCopy[pos.x][pos.y] = boardCopy[pos_.x][pos_.y];
        boardCopy[pos_.x][pos_.y] = {Colour::none, Type::none, nullptr};
        if (isLegalMove(board, pos)) {
            if (isCheck(boardCopy, pos))
                continue;
            possibleMoves_.emplace_back(pos);
        }
    }
}

TypePiece King::getType() {
    return {color_, Type::king, this, first_};
}

Pawn::Pawn(const Coord &pos, Colour color) : Piece(pos, color) {
    int direction = color == Colour::white ? 1 : -1;
    legalMoves_.emplace_back(Coord{0, direction});
    legalMoves_.emplace_back(Coord{0, direction});
    legalMoves_.emplace_back(Coord{1, direction});
    legalMoves_.emplace_back(Coord{-1, direction});
}

bool Pawn::isLegalMove(const TypePiece board[8][8], Coord pos) {
    int direction = color_ == Colour::white ? 1 : -1;
    auto piece = board[pos.x][pos.y];
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    if (pos.x == pos_.x && pos.y == pos_.y + direction) {
        return piece.type == Type::none;
    }
    if (pos.x == pos_.x && pos.y == pos_.y + 2 * direction && first_ == 0) {
        return piece.type == Type::none && board[pos.x][pos.y - direction].type == Type::none;
    }
    if ((pos.x == pos_.x + 1 || pos.x == pos_.x - 1) && pos.y == pos_.y + direction) {
        if (piece.type != Type::none)
            return piece.color != color_; // for eating
        auto pawn = dynamic_cast<Pawn *>(board[pos.x][pos_.y].piece);
        if (pawn == nullptr)
            return false;
        if (pawn->color_ != color_ && pawn->first_ == 1)
            return true;
    }
    return false;
}

bool Pawn::move(const TypePiece board[8][8], const Coord &pos) {
    bool res = Piece::move(board, pos);
    if (!res)
        return false;
    if (pos.x == pos_.x + 1 || pos.x == pos_.x - 1) {
        if (board[pos.x][pos.y].type == Type::none && board[pos.x][pos_.y].type == Type::pawn) {
            board[pos.x][pos.y].piece->kill();
        }
    }
    first_ = (first_ == -1) ? -1 : 0;
    if (pos.x == pos_.x + 2 || pos.x == pos_.x - 2)
        first_ = 1;
    return true;
}

void Pawn::update(const TypePiece board[8][8]) {
    Piece::update(board);
    first_ = (first_ == 1 | first_ == -1) ? -1 : 0;
}

TypePiece Pawn::getType() {
    return {color_, Type::pawn, this, first_};
}


Knight::Knight(const Coord &pos, Colour color) : Piece(pos, color) {
    legalMoves_.emplace_back(Coord{2, 1});
    legalMoves_.emplace_back(Coord{2, -1});
    legalMoves_.emplace_back(Coord{-2, 1});
    legalMoves_.emplace_back(Coord{-2, -1});
    legalMoves_.emplace_back(Coord{1, 2});
    legalMoves_.emplace_back(Coord{1, -2});
    legalMoves_.emplace_back(Coord{-1, 2});
    legalMoves_.emplace_back(Coord{-1, -2});
}

bool Knight::isLegalMove(const TypePiece board[8][8], Coord pos) {
    if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0) {
        return false;
    }
    auto piece = board[pos.x][pos.y];
    if (piece.type != Type::none && piece.color == color_) {
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

TypePiece Knight::getType() {
    return {color_, Type::knight, this};
}
