#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

void printBoard(TypePiece board[8][8]) {
    cout << "***************" << endl;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << board[j][i] << " ";
        }
        cout << endl;
    }
    cout << "***************" << endl;
}

extern void copyBoard(const TypePiece board[8][8], TypePiece boardCopy[8][8], Coord swap = Coord{-1, -1},
                      Coord swap2 = Coord{-1, -1});

Board::Board(Vector2f tileSize) : pieceSelected(nullptr) {
    tile_size = tileSize;
    player.emplace_back(Colour::black);
    player.emplace_back(Colour::white);

    layer1.setPrimitiveType(Quads);
    layer1.resize(8 * 8 * 4);
    layer2.setPrimitiveType(Quads);
    layer2.resize(8 * 8 * 4);
    layer3.setPrimitiveType(Quads);
    layer3.resize(8 * 8 * 4);
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
    Board_game();
}

int Board::Load(std::string *s) {
    if (!chess.loadFromFile(*s))
        return false;
    selection_box.setSize(Vector2f(tile_size.x, tile_size.y));
    selection_box.setFillColor(Color(165, 166, 240));
    selection_box.setPosition(-72, -72);
    check_box.setSize(Vector2f(tile_size.x, tile_size.y));
    check_box.setFillColor(Color(224, 78, 74, 200));
    check_box.setPosition(-72, -72);
    box1.setTexture(chess);
    box1.setTextureRect(IntRect(7 * tile_size.x, 1 * tile_size.y, tile_size.x, tile_size.y));
    box1.setPosition(-72, -72);
    box2.setTexture(chess);
    box2.setTextureRect(IntRect(7 * tile_size.x, 1 * tile_size.y, tile_size.x, tile_size.y));
    box2.setPosition(-72, -72);
    Board_game();
    return true;
}

void Board::Update() {
    layer3.clear();
    layer3.setPrimitiveType(Quads);
    layer3.resize(8 * 8 * 4);
    layer2.clear();
    layer2.setPrimitiveType(Quads);
    layer2.resize(8 * 8 * 4);
    std::vector<Coord> moves;
    if (pieceSelected != nullptr)
        if (pieceSelected->getColor() == player_round)
            moves = pieceSelected->getPossibleMoves();
    for (auto &move: moves) {
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
            if (board_game[i][j].type != Type::none) {
                int tv = (int) board_game[i][j].color;
                int tu = (int) board_game[i][j].type;
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

void Board::Selection_Case(int x, int y) {
    Coord pos = {x, y};
    if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
        return;
    if (pieceSelected != nullptr) {
        if (pieceSelected->getColor() != player_round) {
            pieceSelected = nullptr;
            selection_box.setPosition(-72, -72);
        }
    }
    if (board_game[pos.x][pos.y].type != Type::none && board_game[pos.x][pos.y].color == player_round) {
        pieceSelected = board_game[pos.x][pos.y].piece;
        selection_box.setPosition(x * 72, y * 72);
    } else if (selection_box.getPosition().x >= 0) {
        cout << (pieceSelected->getColor() == Colour::white ? "white" : "black") << endl;
        if (pieceSelected->move(board_game, pos)) {
            cout << pieceSelected->getType() << " " << pieceSelected->getPos() << endl;
            box1.setPosition(selection_box.getPosition());
            box2.setPosition(x * 72, y * 72);
            player_round = (player_round == Colour::white ? Colour::black : Colour::white);
            Update_Move();
        } else {
            cout << pos << endl;
            std::cout << "Mouvement impossible" << std::endl;
            cout << pieceSelected->getType() << endl;
            cout << "move possible :" << endl;
            for (auto &i: pieceSelected->getPossibleMoves())
                cout << i << endl;
            cout << "====================" << endl;
            pieceSelected = nullptr;
        }
        selection_box.setPosition(-72, -72);
    }
}

void Board::Update_Move() {
    for (auto & i : board_game) {
        for (auto & j : i) {
            j = {Colour::none, Type::none, nullptr};
        }
    }
    TypePiece boardCopy[8][8];
    player[0].updateBoard(board_game);
    player[1].updateBoard(board_game);
    copyBoard(board_game, boardCopy);
    player[(int) player_round].update(boardCopy);
    check_box.setPosition(-72, -72);
    Coord pos = player[(int) player_round].getKingPos();
    switch (player[(int) player_round].State()) {
        case State::checkmate:
            cout << "Echec et mat" << endl;
            check_box.setPosition(pos.x * 72, pos.y * 72);
            break;
        case State::stalemate:
            cout << "Pat" << endl;
            break;
        case State::check:
            cout << "Echec" << endl;
            check_box.setPosition(pos.x * 72, pos.y * 72);
            break;
        default:
            break;
    }
}

void Board::draw(RenderTarget &target, RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &chess;
    target.draw(layer1, states);
    if (selection_box.getPosition().x >= 0)
        target.draw(selection_box, states);
    if (box1.getPosition().x >= 0)
        target.draw(box1, states);
    if (box2.getPosition().x >= 0)
        target.draw(box2, states);
    if (check_box.getPosition().x >= 0)
        target.draw(check_box, states);
    target.draw(layer2, states);
    target.draw(layer3, states);
}

void Board::Board_game() {


    Update_Move();
}

