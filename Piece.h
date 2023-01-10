#ifndef _PIECE_H
#define _PIECE_H
#include <SFML/Graphics.hpp>
struct Pair
{
	int x, y;
};
class Piece
{
public:
	Piece(int id, int x, int y);
	void Move(const std::vector<Piece*>* board);
	int Type()const;
	int Pos() const;
	void Promote(int _type);
	void Set_Position(int box);
	std::vector<int> get_Move() const;
	bool Check_Chess(const std::vector<Piece*>* board, int box = -1);

private:
	bool Check_Move(const std::vector<Piece*>* board, int x, int y, bool eat = true, bool need_eat = false);
	int Check_Move(int p, const std::vector<Piece*>* board, int x, int y);
	Pair pos;
	int type;
	bool first;
	std::vector<int> move;
};
#endif // !PIECE
