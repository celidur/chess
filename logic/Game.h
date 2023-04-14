//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "common/struct.h"
#include "Player.h"
#include <array>

namespace chess {

    class Game {
    public:
        Game();

        explicit Game(const TypePiece board[8][8], Color color = Color::white);

        virtual ~Game() = default;

        void update();

        virtual void loadGame(Color color);

        void selectionCase(Coord pos);

        void updateBoard(screen::BoardBase& board);

        virtual void resetBoard();
        virtual void setDefaultBoard();

        [[nodiscard]] TypePiece (&getBoard() )[8][8];

    protected:
        virtual void displayMessage(const std::string& msg);
        void promotion(Type type);

    private:
        bool isKingDefined();
        void checkGameState(const Coord &pos);
        void updatePlayerBoard(int playerNumber);

        std::vector<Player> player_;
        TypePiece board_[8][8];
        std::shared_ptr<Piece> pieceBoard_[8][8];
        Color playerRound_;
        std::shared_ptr<Piece> pieceSelected_;
        Coord selection_[4];
        bool rotation_ = true;
        Coord promotionPos_;
        Mode mode_ = Mode::personalisation;

        void clearTypePieceBoard();
    };
}

#endif //CHESS_GAME_H
