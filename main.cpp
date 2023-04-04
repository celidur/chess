#include <SFML/Graphics.hpp>
#include "Board.h"
#include <string>
#include <iostream>
using namespace sf;

int main()
{
    RenderWindow window;
    Board board(Vector2f(72.f, 72.f));
    std::string s = "res/chess.png";
    if (!board.Load(&s))
    {
        return 1;
    }
    window.create(VideoMode(72 * 8, 72 * 8), "Jeu");
    window.setVerticalSyncEnabled(true);
    while (window.isOpen())
    {
        Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    board.Selection_Case(event.mouseButton.x/72*72, event.mouseButton.y/72*72);
                }

            default:
                break;
            }
        }
        window.clear(sf::Color::Black);
        board.Update();
        window.draw(board);
        window.display();
    }
    return 0;
}