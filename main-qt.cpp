#include "Game.h"
#include "Board-qt.h"
#include <string>
#include <QApplication>

constexpr screen::CoordF tileSize{72, 72};

int main(int argc, const char* argv[]) {
    QApplication app(argc, argv);
    chess::Game game;


    screen::Board board(tileSize, (std::string &) "res/chess.png");
    board.show();

    return app.exec();

//    RenderWindow window;
//    std::string s = "res/chess.png";
//    screen::Board board(tileSize, s);
//    chess::Game game;
//    window.create(VideoMode(72 * 8, 72 * 8), "Jeu");
//    window.setVerticalSyncEnabled(true);
//    while (window.isOpen())
//    {
//        Event event{};
//        while (window.pollEvent(event)) {
//            switch (event.type) {
//            case Event::Closed:
//                window.close();
//                break;
//            case Event::MouseButtonPressed:
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    Coord pos = {event.mouseButton.x / (int)tileSize.x, event.mouseButton.y / (int)tileSize.y};
//                    game.selectionCase(pos);
//                }
//            default:
//                break;
//            }
//        }
//        window.clear(sf::Color::Black);
//        game.updateBoard(board);
//        window.draw(board);
//        window.display();
//    }
//    return 0;
}