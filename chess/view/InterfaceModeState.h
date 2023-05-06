#ifndef CHESS_INTERFACEMODESTATE_H
#define CHESS_INTERFACEMODESTATE_H

#include "IBoard.h"
#include "common/struct.h"


namespace view {

    class InterfaceModeState {
    public:
        explicit InterfaceModeState(Mode mode) : mode_(mode) {}


        virtual void handleMouseEvent(view::IBoard &, Coord &) = 0;

        virtual void
        handleUpdateEvent(view::IBoard &, const Coord selection[4], const std::array<std::array<TypePiece, xBoard>, yBoard>& board,
                          const std::vector<Coord> &piecePossibleMove, Color color,
                          const std::vector<TypePiece> deadPieces[2],
                          int point) = 0;

        virtual void handleUpdateEvent(view::IBoard &, const std::array<std::array<TypePiece, xBoard>, yBoard>& board) = 0;

        virtual void handleUpdatePanel(IBoard &board) = 0;

        virtual void handleUpdatePanel(IBoard &board, const std::vector<TypePiece> deadPieces[2], int point) = 0;


        [[nodiscard]] Mode getMode() const {
            return mode_;
        }

    private:
        Mode mode_;
    };

} // view


#endif //CHESS_INTERFACEMODESTATE_H
