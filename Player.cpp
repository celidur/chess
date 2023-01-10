#include "Player.h"
#include <iostream>

Player::Player(int color)
{
	nb_move = 0;
	player_color = color;
	for (int i = 0; i < 16; i++)
	{
		if (i < 8)
			piece.push_back(Piece(0 + 6 * color, i, 1 + 5 * color));
		else if (i == 8 or i == 15)
			piece.push_back(Piece(1 + 6 * color, i % 8, 7 * color));
		else if (i == 9 or i == 14)
			piece.push_back(Piece(2 + 6 * color, i % 8, 7 * color));
		else if (i == 10 or i == 13)
			piece.push_back(Piece(3 + 6 * color, i % 8, 7 * color));
		else if (i == 11)
			piece.push_back(Piece(4 + 6 * color, i % 8, 7 * color));
		else
		{
			piece.push_back(Piece(5 + 6 * color, i % 8, 7 * color));
		}
	}
}


void Player::Player_mov(std::vector<Piece*>* board)
{
	nb_move = 0;
	chess = !piece[0].Check_Chess(board);
	for (int i = 0; i < piece.size(); i++)
	{
		piece[i].Move(board);
		nb_move += piece[i].get_Move().size();
	}
}

std::vector<Piece>* Player::Player_Piece()
{
	return &piece;
}

void Player::Delete_Piece(Piece* p)
{
	for (int i = 0; i < piece.size(); i++)
	{
		if (&piece[i] == p)
		{
			piece.erase(piece.begin() + i);
			break;
		}
	}
}

int Player::State()
{
	if (nb_move == 0 and chess)
	{
		return 1;
	}
	else if (nb_move == 0)
	{
		return 2;
	}
	else if (chess)
	{
		return 3;
	}
	return 0;
}
