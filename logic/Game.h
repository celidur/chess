/**
* \file   Game.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Player.h"
#include <array>

namespace logic {

    class Game {
    public:
        Game();

        explicit Game(const TypePiece board[8][8], Color color = Color::white);

        virtual ~Game() = default;

        void update();

        virtual void loadGame();

        void selectionCase(Coord pos);

        void updateBoard();

        virtual void resetBoard();

        virtual void setDefaultBoard();

        [[nodiscard]] Mode getMode();

        [[nodiscard]] bool isRotation() const;

        void setRotation(bool rotation);

        void setPlayerRound(Color playerRound);

    protected:
        virtual void displayMessage(const std::string &msg);

        virtual void
        updateGameBoard(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove,
                        Color color, std::vector<TypePiece> deadPieces[2]) = 0;

        virtual void viewBoard(Color color) = 0;

        virtual void updatePersonalizationBoard(TypePiece boardGame[8][8]) = 0;

        void promotion(Type type);

        void addPiece(Coord &pos, TypePiece &type);

    private:
        bool isKingDefined();

        void checkGameState();

        void clearTypePieceBoard();

        std::vector<Player> player_;
        TypePiece board_[8][8];
        Color playerRound_;
        Coord selection_[4];
        bool rotation_;
        Coord promotionPos_;
        Mode mode_;
    };
}

#endif //CHESS_GAME_H
