//
// Created by Gaetan on 2023-04-05.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Player.h"
#include <array>

namespace chess {

    class Game {
    public:
        Game();

        explicit Game(const TypePiece board[8][8], Color color = Color::white);

        virtual ~Game() = default;

        void update();

        virtual void loadGame();

        void selectionCase(Coord pos);

        void updateBoard(screen::BoardBase& board);

        virtual void resetBoard();
        virtual void setDefaultBoard();

        [[nodiscard]] TypePiece (&getBoard() )[8][8];
        [[nodiscard]] Mode getMode();

        [[nodiscard]] bool isRotation() const;

        void setRotation(bool rotation);

        void setMode(Mode mode);

        Color getPlayerRound() const;

        void setPlayerRound(Color playerRound);

        [[nodiscard]] const Coord &getSelectedCoord() const;

        void setSelectedCoord(const Coord &selectedCoord);

    protected:
        virtual void displayMessage(const std::string& msg);
        void promotion(Type type);
        void addPiece(Coord &pos, TypePiece &type);

    private:
        bool isKingDefined();
        void checkGameState(const Coord &pos);
        void updatePlayerBoard(int playerNumber);
        void clearTypePieceBoard();

        std::vector<Player> player_;
        TypePiece board_[8][8];
        Color playerRound_;
        Piece *pieceSelected_;
        Coord selection_[4];
        bool rotation_ = true;
        Coord promotionPos_;
        Mode mode_ = Mode::personalisation;
        Coord selectedCoord_ = {8, 7};
        Color selectedColor_ = Color::white;
    };
}

#endif //CHESS_GAME_H
