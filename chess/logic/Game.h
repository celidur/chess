/**
* \file   Game.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
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

        void selectionCase(Coord& pos);

        void resetBoard();

        void setDefaultBoard();

        [[nodiscard]] Mode getMode();

        [[nodiscard]] bool isRotation() const;

        [[nodiscard]] std::array<std::array<TypePiece, xBoard>, yBoard>& getBoard();

        void setRotation(bool rotation);

        void setPlayerRound(const Color& playerRound);

        virtual void viewBoard(const Color& color) = 0;

        virtual void updateCheck() = 0;

        virtual void updatePanel() = 0;

    protected:
        virtual void displayMessage(const std::string& msg);

        virtual void killPiece(const Coord& pos) = 0;

        virtual void movePiece(Coord& pos1, Coord& pos2) = 0;

        virtual void showPromotion() = 0;

        void promotion(Type type);

        void addPiece(const Coord& pos, const TypePiece& type);

        virtual void updateSelection(const Coord& pos, const std::vector<Coord>& piecePossibleMove) = 0;

    protected:

        void checkGameState();

        void clearTypePieceBoard();

        std::vector<Player> player_;
        std::array<std::array<TypePiece, xBoard>, yBoard> board_;
        Color playerRound_;
        Coord selection_[4];
        bool rotation_;
        Coord promotionPos_;
        Mode mode_;
    };
}

#endif //CHESS_GAME_H
