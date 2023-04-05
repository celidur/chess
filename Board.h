#ifndef BOARD
#define	BOARD
#include "Player.h"
#include <SFML/Graphics.hpp>

class Board : public sf::Drawable, public sf::Transformable
{
public:
	explicit Board(sf::Vector2f tileSize);
	~Board() override = default;
	int Load(std::string* s);
	void Board_game();
	void Update();
	void Selection_Case(int x, int y);

private:
	void Update_Move();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override ;
	sf::Texture chess;
	sf::VertexArray layer1;
	sf::VertexArray layer2;
	sf::VertexArray layer3;
    sf::RectangleShape selection_box;
    sf::RectangleShape check_box;
	sf::Sprite box1;
	sf::Sprite box2;
	sf::Vector2f tile_size;
    Piece* pieceSelected;
	std::vector<Player> player;
	TypePiece board_game[8][8];
	Colour player_round = Colour::white;
};

#endif // !BOARD
