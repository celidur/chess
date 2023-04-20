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

        virtual ~Game() = default;

        void update();

        virtual void loadGame();

        void selectionCase(Coord pos);

        static void resetBoard(TypePiece board[xBoard][yBoard]);

        static void setDefaultBoard(TypePiece board[xBoard][yBoard]);

        [[nodiscard]] Mode getMode();

        [[nodiscard]] bool isRotation() const;

        void setRotation(bool rotation);

        void setPlayerRound(Color playerRound);

        virtual void viewBoard(Color color) = 0;

        virtual void updateCheck() = 0;

        virtual void updatePanel() = 0;

    protected:
        virtual void displayMessage(const std::string& msg);

        virtual void killPiece(Coord& pos) = 0;

        virtual void movePiece(Coord& pos1, Coord& pos2) = 0;

        virtual void showPromotion() = 0;

        void promotion(Type type);

        void addPiece(Coord& pos, TypePiece& type);

        virtual void updateSelection(Coord& pos, std::vector<Coord>& piecePossibleMove) = 0;

    protected:
        bool isKingDefined();

        void checkGameState();

        void clearTypePieceBoard();

        std::vector<Player> player_;
        TypePiece board_[xBoard][yBoard];
        Color playerRound_;
        Coord selection_[4];
        bool rotation_;
        Coord promotionPos_;
        Mode mode_;
    };
}

#endif //CHESS_GAME_H
