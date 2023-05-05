#ifndef CHESS_PERSONNALISATIONMODE_HPP
#define CHESS_PERSONNALISATIONMODE_HPP

#include "InterfaceModeState.hpp"

namespace view {

    class PersonnalisationMode : public InterfaceModeState{
    public:
        explicit PersonnalisationMode();

        void handleMouseEvent(IBoard &board, Coord &coord) override;

        void handleUpdateEvent(IBoard &board, const Coord *selection, const TypePiece (*boardGame)[8],
                               const std::vector<Coord> &piecePossibleMove, Color color,
                               const std::vector<TypePiece> *deadPieces, int point) override;

        void handleUpdateEvent(IBoard &board, const TypePiece (*boardGame)[8]) override;

        void handleUpdatePanel(IBoard& board) override;

        void handleUpdatePanel(IBoard& board, const std::vector<TypePiece> deadPieces[2], int point) override;


    private:
        TypePiece selectedPiece_;
        Color selectedColor_;
    };

} // view

#endif //CHESS_PERSONNALISATIONMODE_HPP
