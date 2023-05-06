#ifndef CHESS_PERSONNALISATIONMODE_H
#define CHESS_PERSONNALISATIONMODE_H

#include "InterfaceModeState.h"

namespace view {

    static const auto selectedPieceColor = QColor::fromRgb(180, 150, 140);
    static const auto inactivePieceColor = QColor::fromRgb(209, 207, 206);
    static const auto switchBtnColor = QColor::fromRgb(70, 100, 130);
    static const auto playBtnColor = QColor::fromRgb(100, 70, 80);
    static const auto defaultBtnColor = QColor::fromRgb(180, 150, 140);
    static const auto resetBtnColor = QColor::fromRgb(100, 200, 80);
    static const auto firstPlayerBtnColor = QColor::fromRgb(100, 70, 200);
    static const auto rotationBtnColor  = QColor::fromRgb(150, 20, 200);

    class PersonnalisationMode : public InterfaceModeState{
    public:
        explicit PersonnalisationMode();

        void handleMouseEvent(IBoard &board, Coord &coord) override;

        void handleUpdateEvent(IBoard &board, const Coord *selection, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame,
                               const std::vector<Coord> &piecePossibleMove, Color color,
                               const std::vector<TypePiece> *deadPieces, int point) override;

        void handleUpdateEvent(IBoard &board, const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) override;

        void handleUpdatePanel(IBoard& board) override;

        void handleUpdatePanel(IBoard& board, const std::vector<TypePiece> deadPieces[2], int point) override;


    private:
        TypePiece selectedPiece_;
        Color selectedColor_;
    };

} // view

#endif //CHESS_PERSONNALISATIONMODE_H
