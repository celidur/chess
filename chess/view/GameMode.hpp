#ifndef CHESS_GAMEMODE_HPP
#define CHESS_GAMEMODE_HPP

#include "InterfaceModeState.hpp"

namespace view {
    class GameMode : public InterfaceModeState {
    public:
        GameMode();

        void handleMouseEvent(view::IBoard &, Coord &) override;

        void handleUpdateEvent(view::IBoard &, const Coord *selection, const TypePiece (*boardGame)[8],
                               const std::vector<Coord> &piecePossibleMove, Color color,
                               const std::vector<TypePiece> *deadPieces, int point) override;

        void handleUpdateEvent(view::IBoard &, const TypePiece (*boardGame)[8]) override;

        void handleUpdatePanel(IBoard& board) override;

        void handleUpdatePanel(IBoard& board, const std::vector<TypePiece> deadPieces[2], int point) override;

    };
}

#endif //CHESS_GAMEMODE_HPP
