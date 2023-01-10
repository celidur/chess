#ifndef BOARD
#define	BOARD
#include "Player.h"
class Board : public sf::Drawable, public sf::Transformable
{
public:
	Board(sf::Vector2f tileSize);
	~Board();
	int Load(std::string* s);
	void Board_game();
	void Update();
	void Selection_Case(int x, int y);

private:
	void Update_Move();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const ;
	sf::Texture chess;
	sf::VertexArray layer1;
	sf::VertexArray layer2;
	sf::VertexArray layer3;
	sf::Sprite selection_box;
	sf::Sprite box1;
	sf::Sprite box2;
	sf::Vector2f tile_size;
	std::vector<Player> player;
	std::vector<Piece*> board_game;
	int player_round = 1;
};

#endif // !BOARD
