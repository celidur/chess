#include "Piece.h"

Piece::Piece(const Coord &pos, Colour color) : pos_(pos), color_(color) {}

bool Piece::move(const std::vector<Piece *> &board, const Coord &pos) {
    if (isLegalMove(board, pos)) {
        pos_ = pos;
        return true;
    }
    return false;
}

void Piece::Move(const std::vector<Piece*>* board)
{
	move.clear();
	switch (type % 6)
	{
	case 0:
		if (first)
		{
			if ((*board)[pos.x + (pos.y + 1 - 2 * (type / 6)) * 8] == 0)
				Check_Move(board, pos.x, pos.y + 2 - 4 * (type / 6), false);
		}
		Check_Move(board, pos.x, pos.y + 1 - 2 * (type / 6), false);
		Check_Move(board, pos.x + 1, pos.y + 1 - 2 * (type / 6), true, true);
		Check_Move(board, pos.x - 1, pos.y + 1 - 2 * (type / 6), true, true);
		break;
	case 1:
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < 8; j++)
			{
				if (Check_Move(board, pos.x + pow(-1, i / 2) * (i % 2) * j, pos.y + pow(-1, i / 2 + 1) * j * ((i + 1) % 2)))
					break;
			}
		}
		break;
	case 2:
		for (int i = 0; i < 8; i++)
			Check_Move(board, pos.x + (pow(-1, i % 2) * ((i + 4) / 4)), pos.y + (pow(-1, i / 2) * (((i / 4 + 1) % 2) + 1)));
		break;
	case 3:
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < 8; j++)
			{
				if (Check_Move(board, pos.x + pow(-1, i / 2) * j, pos.y + pow(-1, i % 2) * j))
					break;
			}
		}
		break;
	case 4:
		for (int i = 0; i < 8; i++)
		{
			for (int j = 1; j < 8; j++)
			{
				if (i / 4 == 0)
				{
					if (Check_Move(board, pos.x + pow(-1, i / 2) * (i % 2) * j, pos.y + pow(-1, i / 2 + 1) * j * ((i + 1) % 2)))
						break;
				}
				else
				{
					if (Check_Move(board, pos.x + pow(-1, i / 2) * j, pos.y + pow(-1, i % 2) * j))
						break;
				}
			}
		}
		break;
	case 5:
		for (int i = 0; i < 8; i++)
		{
			if (i / 4 == 0)
			{
				Check_Move(board, pos.x + pow(-1, i / 2) * (i % 2), pos.y + pow(-1, i / 2 + 1) * ((i + 1) % 2));
			}
			else
			{
				Check_Move(board, pos.x + pow(-1, i / 2), pos.y + pow(-1, i % 2));
			}
		}
		if (first)
		{// gauche
			if ((*board)[pos.x - 1 + pos.y * 8] == 0 and (*board)[pos.x - 2 + pos.y * 8] == 0 and (*board)[pos.x - 3 + pos.y * 8] == 0 and Check_Move(1, board, pos.x - 4, pos.y) == 3)
			{
				if ((*board)[pos.x - 4 + pos.y * 8]->first)
					Check_Move(board, pos.x - 2, pos.y);
			}
			if ((*board)[pos.x + 1 + pos.y * 8] == 0 and (*board)[pos.x + 2 + pos.y * 8] == 0 and Check_Move(1, board, pos.x + 3, pos.y) == 3)
			{
				if ((*board)[pos.x + 3 + pos.y * 8]->first)
					Check_Move(board, pos.x + 2, pos.y);
			}
		}
		break;
	default:
		break;
	}
}

int Piece::Type()const
{
	return type;
}

int Piece::Pos() const
{
	return pos.y * 8 + pos.x;
}

void Piece::Promote(int _type)
{
	type = _type + 6 * (type / 6);
}

void Piece::Set_Position(int box)
{
	pos.x = box % 8;
	pos.y = box / 8;
	first = false;
}

std::vector<int> Piece::get_Move() const
{
	return move;
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

bool Piece::Check_Move(const std::vector<Piece*>* board, int x, int y, bool eat, bool need_eat) // ajouter verification echec a découverte 
{
	
	if (x >= 0 and x < 8 and y < 8 and y >= 0)
	{
		int box = x + y * 8;
		if ((*board)[box] == 0)
		{
			if (!need_eat)
			{
				if (Check_Chess(board, box))
				{
					move.push_back(box);
				}
			}
			return false;
		}
		else
		{
			if ((*board)[box]->type / 6 != type / 6 and eat)
			{
				if (Check_Chess(board, box))
				{
					move.push_back(box);
				}
			}
			return true;
		}
	}
	return true;
}

int Piece::Check_Move(int p, const std::vector<Piece*>* board, int x, int y)
{
	if (x >= 0 and x < 8 and y < 8 and y >= 0)
	{
		int box = x + y * 8;
		if ((*board)[box] == 0)
			return 1;
		if ((*board)[box] != 0)
		{
			if ((*board)[box]->type / 6 != type / 6 and (*board)[box]->type % 6 == p)
				return 0;
			if ((*board)[box]->type % 6 == p)
				return 3;
		}
	}
	return 2;
}

bool Piece::Check_Chess(const std::vector<Piece*>* board, int box)
{
	std::vector<Piece*> board2;
	for (int i = 0; i < board->size(); i++)
	{
		board2.push_back(board->operator[](i));
	};
	int box2 = pos.x + pos.y * 8;
	if (box>=0)
	{
		board2[box] = board2[box2];
		board2[box2] = 0;
	}
	Pair king2{ -1,-1};
	for (int i = 0; i < 64; i++)
	{
		if (board2[i] != 0)
		{
			if (board2[i]->type == (type / 6)*6 + 5)
			{
				king2.x = i % 8;
				king2.y = i / 8;
			}
		}
	}
	// pion
	if (Check_Move(0, &board2,king2.x + 1 , king2.y + 1 - 2 * (type/6))==0)
	{
		return false;
	}
	if (Check_Move(0, &board2, king2.x - 1, king2.y + 1 - 2 * (type/6))==0)
	{
		return false;
	}
	// tour/dame
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			for (int j = 1; j < 8; j++)
			{
				int a = Check_Move(1 + i * 3, &board2, king2.x + pow(-1, k / 2) * (k % 2) * j, king2.y + pow(-1, k / 2 + 1) * j * ((k + 1) % 2));
				if (a >= 2)
					break;
				if (a == 0)
					return false;
			}
		}
		
	}
	// fou/dame
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			for (int j = 1; j < 8; j++)
			{
				int a = Check_Move(3 + i * 1, &board2, king2.x + pow(-1, k / 2) * j, king2.y + pow(-1, k % 2) * j);
				if (a >= 2)
					break;
				if (a == 0)
					return false;
			}
		}
	}
	// cavalier
	for (int i = 0; i < 8; i++)
	{
		if (Check_Move(2, &board2, king2.x + (pow(-1, i % 2) * ((i + 4) / 4)), king2.y + (pow(-1, i / 2) * (((i / 4 + 1) % 2) + 1))) == 0)
			return false;
	}
	// roi
	for (int i = 0; i < 8; i++)
	{
		if (i / 4 == 0)
		{
			if (Check_Move(5, &board2, king2.x + pow(-1, i / 2) * (i % 2), king2.y + pow(-1, i / 2 + 1) * ((i + 1) % 2)) == 0)
				return false;
		}
		else
		{
			if (Check_Move(5, &board2, king2.x + pow(-1, i / 2), king2.y + pow(-1, i % 2)) == 0)
				return false;
		}
	}
	return true;
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
