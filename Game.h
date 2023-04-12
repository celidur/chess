//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Player.h"
#include "Board.h"

namespace chess {
    class Game {
    public:
        Game();

        explicit Game(const screen::TypePiece board[8][8], Color color = Color::white);

        virtual ~Game() = default;

        void update();

        void loadGame(const screen::TypePiece board[8][8], Color color = Color::white);

        void selectionCase(Coord pos);

        void updateBoard(screen::Board& board);

        [[nodiscard]] screen::TypePiece(& getBoard() const)[8][8];

    protected:
        virtual void displayMessage(std::string msg);

        void promotion(Type type);

    private:
        std::vector<Player> player_;
        TypePiece board_[8][8];
        Color playerRound_;
        Piece *pieceSelected_;
        Coord selection_[4];
        bool rotation_ = true;
        Coord promotionPos_;
    };
}

#endif //CHESS_GAME_H
