#ifndef CHESS_GAMEMODE_H
#define CHESS_GAMEMODE_H

#include "InterfaceModeState.h"

namespace view {
    class GameMode : public InterfaceModeState {
    public:
        GameMode();

        void handleMouseEvent(view::IBoard &, Coord &) override;

        void handleUpdateEvent(view::IBoard &, const Coord *selection, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame,
                               const std::vector<Coord> &piecePossibleMove, Color color,
                               const std::vector<TypePiece> *deadPieces, int point) override;

        void handleUpdateEvent(view::IBoard &, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) override;

        void handleUpdatePanel(IBoard& board) override;

        void handleUpdatePanel(IBoard& board, const std::vector<TypePiece> deadPieces[2], int point) override;

    };
}

#endif //CHESS_GAMEMODE_H
