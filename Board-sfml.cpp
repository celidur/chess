#include "Board-sfml.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

namespace screen {
    Board::Board(CoordF tileSize, std::string& s) : layer1(Quads, 8 * 8 * 4),
                                                    layer2(Quads, 8 * 8 * 4), layer3(Quads, 8 * 8 * 4),
                                                    tile_size({tileSize.x, tileSize.y}) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int tu = 6;
                int tv = (i % 2 + j % 2) % 2;
                // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                Vertex *quad = &layer1[(i + j * 8) * 4];

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
        if (!chess.loadFromFile(s))
            throw std::runtime_error("Error loading image");
    }

    void Board::update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove) {
        for (int i = 0; i < 4; ++i) {
            selection_[i] = selection[i];
        }
        layer3.clear();
        layer3.setPrimitiveType(Quads);
        layer3.resize(8 * 8 * 4);
        layer2.clear();
        layer2.setPrimitiveType(Quads);
        layer2.resize(8 * 8 * 4);
        for (auto &move: piecePossibleMove) {
            int tv = 0;
            int tu = 7;
            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
            int x = move.x;
            int y = move.y;
            Vertex *quad = &layer3[(x + y * 8) * 4];
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

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (boardGame[i][j].type != Type::none) {
                    int tv = (int) boardGame[i][j].color;
                    int tu = (int) boardGame[i][j].type;
                    // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                    Vertex *quad = &layer2[(i + j * 8) * 4];

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

    void Board::draw(RenderTarget &target, RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &chess;
        target.draw(layer1, states);
        sf::RectangleShape selection_box;
        selection_box.setSize(Vector2f(tile_size.x, tile_size.y));
        selection_box.setFillColor(Color(165, 166, 240));
        for (auto &i: selection_) {
            selection_box.setPosition(i.x * tile_size.x, i.y * tile_size.y);
            if (&selection_[3] == &i)
                selection_box.setFillColor(Color(224, 78, 74, 200));
            if (i.x >= 0)
                target.draw(selection_box, states);
        }
        target.draw(layer2, states);
        target.draw(layer3, states);
    }
}
