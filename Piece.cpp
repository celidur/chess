#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

Piece::Piece(int id, int x, int y)
{
	first = true;
	type = id;
	pos.x = x;
	pos.y = y;
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
