#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
Board::Board(Vector2f tileSize)
{
	tile_size = tileSize;
	player.push_back(Player(0));
	player.push_back(Player(1));
	layer1.setPrimitiveType(Quads);
	layer1.resize(8 * 8 * 4);
	layer2.setPrimitiveType(Quads);
	layer2.resize(8 * 8 * 4);
	layer3.setPrimitiveType(Quads);
	layer3.resize(8 * 8 * 4);
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			int tu = 6;
			int tv = (i % 2 + j % 2) % 2;

			// on récupère un pointeur vers le quad à définir dans le tableau de vertex
			Vertex* quad = &layer1[(i + j * 8) * 4];

			// on définit ses quatre coins
			quad[0].position = Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// on définit ses quatre coordonnées de texture
			quad[0].texCoords = Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}
	Board_game();
}

Board::~Board()
{
}

int Board::Load(std::string* s)
{
	if (!chess.loadFromFile(*s))
		return false;
	selection_box.setTexture(chess);
	selection_box.setTextureRect(IntRect(7 * tile_size.x, 1 * tile_size.y, tile_size.x, tile_size.y));
	selection_box.setPosition(-72, -72);
	box1.setTexture(chess);
	box1.setTextureRect(IntRect(7 * tile_size.x, 1 * tile_size.y, tile_size.x, tile_size.y));
	box1.setPosition(-72, -72);
	box2.setTexture(chess);
	box2.setTextureRect(IntRect(7 * tile_size.x, 1 * tile_size.y, tile_size.x, tile_size.y));
	box2.setPosition(-72, -72);
	Board_game();
	return true;
}

void Board::Update()
{
	layer3.clear();
	layer3.setPrimitiveType(Quads);
	layer3.resize(8 * 8 * 4);
	layer2.clear();
	layer2.setPrimitiveType(Quads);
	layer2.resize(8 * 8 * 4);
	std::vector<int> move;
	if (selection_box.getPosition().x >= 0)
		move = board_game[selection_box.getPosition().x / 72 + selection_box.getPosition().y / 72 * 8]->get_Move();
	for (int i = 0; i < move.size(); i++)
	{
		int tv = 0;
		int tu = 7;
		// on récupère un pointeur vers le quad à définir dans le tableau de vertex
		Vertex* quad = &layer3[move[i]*4];
		int x = move[i] % 8;
		int y = move[i] / 8;
		// on définit ses quatre coins
		quad[0].position = Vector2f(x * tile_size.x, y * tile_size.y);
		quad[1].position = Vector2f((x + 1) * tile_size.x, y * tile_size.y);
		quad[2].position = Vector2f((x + 1) * tile_size.x, (y + 1) * tile_size.y);
		quad[3].position = Vector2f(x * tile_size.x, (y + 1) * tile_size.y);

		// on définit ses quatre coordonnées de texture
		quad[0].texCoords = Vector2f(tu * tile_size.x, tv * tile_size.y);
		quad[1].texCoords = Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
		quad[2].texCoords = Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
		quad[3].texCoords = Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);

	}
	
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board_game[i + j * 8] != 0)
			{
				int tv = board_game[i + j * 8]->Type() / 6;
				int tu = board_game[i + j * 8]->Type() % 6;
				// on récupère un pointeur vers le quad à définir dans le tableau de vertex
				Vertex* quad = &layer2[(i + j * 8) * 4];

				// on définit ses quatre coins
				quad[0].position = Vector2f(i * tile_size.x, j * tile_size.y);
				quad[1].position = Vector2f((i + 1) * tile_size.x, j * tile_size.y);
				quad[2].position = Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
				quad[3].position = Vector2f(i * tile_size.x, (j + 1) * tile_size.y);

				// on définit ses quatre coordonnées de texture
				quad[0].texCoords = Vector2f(tu * tile_size.x, tv * tile_size.y);
				quad[1].texCoords = Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
				quad[2].texCoords = Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
				quad[3].texCoords = Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
			}
		}
	}
}

void Board::Selection_Case(int x, int y)
{
	int box = x / 72 + y / 72 * 8;
	int pos = selection_box.getPosition().x / 72 + selection_box.getPosition().y / 72 * 8;
	selection_box.setPosition(-72, -72);
	if (box < board_game.size() and box >= 0)
		if (pos >= 0)
			for (int i = 0; i < board_game[pos]->get_Move().size(); i++)
			{
				if (board_game[pos]->get_Move().operator[](i)==box)
				{
					player_round += 1;
					player_round %= 2;
					if (board_game[box])
					{
						player[player_round].Delete_Piece(board_game[box]);
					}
					board_game[pos]->Set_Position(box);
					// promotion 
					if (board_game[pos]->Type() % 6 == 0 and (box / 8) % 7 == 0)
						board_game[pos]->Promote(4);
					if (box-pos==2 and board_game[pos]->Type() % 6 == 5)
					{
						board_game[pos + 3]->Set_Position(box - 1);
					}
					else if (pos - box == 2 and board_game[pos]->Type() % 6 == 5)
					{
						board_game[pos - 4]->Set_Position(box + 1);
					}
					Board_game();
					box1.setPosition(x, y);
					box2.setPosition(pos % 8 * 72, pos / 8 * 72);
					return;
				}
			}
		if (board_game[box] != 0)
			if (board_game[box]->Type() / 6 == player_round)
				selection_box.setPosition(x, y);
}

void Board::Update_Move()
{
	player[player_round].Player_mov(&board_game);
	switch (player[player_round].State())
	{
	case 1:
		cout << "Echec et mat" << endl;
		break;
	case 2:
		cout << "Pat" << endl;
		break;
	case 3:
		cout << "Echec" << endl;
		break;
	default:
		break;
	}
}

void Board::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &chess;
	target.draw(layer1, states);
	if (selection_box.getPosition().x >= 0)
		target.draw(selection_box, states);
	if (box1.getPosition().x >= 0)
		target.draw(box1, states);
	if (box2.getPosition().x >= 0)
		target.draw(box2, states);
	target.draw(layer2, states);
	target.draw(layer3, states);
}

void Board::Board_game()
{
	board_game.clear();
	for (int i(0);i<64;++i)
	{
		board_game.push_back(0);
	}
	for (size_t j = 0; j < 2; j++)
	{
		std::vector<Piece>* player_piece = player[j].Player_Piece();
		for (int i(0); i < player_piece->size(); ++i)
		{
			Piece* piece = &player_piece->operator[](i);
			board_game[piece->Pos()] = piece;
		}		
	}
	Update_Move();
}