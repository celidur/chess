#ifndef PLAYER
#define PLAYER
#include <SFML/Graphics.hpp>
#include "Piece.h"

class Player
{
public:
	Player(int color = 0);
	void Player_mov(std::vector<Piece*>* board);
	std::vector<Piece>* Player_Piece();
	void Delete_Piece(Piece* p);
	int State();
private:
	int player_color;
	std::vector<Piece> piece;
	int nb_move;
	bool chess;
};

#endif // !PLAYER


