#ifndef BOARD
#define    BOARD

#include <SFML/Graphics.hpp>
#include "struct.h"
#include "BoardBase.hpp"

namespace screen {


    class Board : public BoardBase, public sf::Drawable, public sf::Transformable {
    public:
        Board(CoordF tileSize, std::string& s);

        ~Board() override = default;

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove) override;

    private:

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        sf::Texture chess;
        sf::VertexArray layer1;
        sf::VertexArray layer2;
        sf::VertexArray layer3;
        sf::Vector2f tile_size;
        Coord selection_[4];
    };

}

#endif // !BOARD
