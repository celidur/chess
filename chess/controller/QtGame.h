/**
* \file   QtGame.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#ifndef CHESS_QTGAME_H
#define CHESS_QTGAME_H

#include <QObject>
#include <QMessageBox>
#include "logic/Game.h"
#include "view/Board.h"


namespace controller {
    class QtGame : public QObject, public logic::Game {
    Q_OBJECT

    protected:
        void displayMessage(const std::string& msg) override; // Addition to display the message in a system box

        void
        updateGameBoard(Coord selection[4], std::array<std::array<TypePiece, xBoard>, yBoard>&, std::vector<Coord>& piecePossibleMove,
                        Color color, std::vector<TypePiece> deadPieces[2], int point);

        void viewBoard(const Color& color) override;

        void updatePersonalizationBoard(const std::array<std::array<TypePiece, xBoard>, yBoard>& board);

        void killPiece(const Coord& pos) override;

        void movePiece(Coord& pos1, Coord& pos2) override;

        void updateSelection(const Coord& pos, const std::vector<Coord>& piecePossibleMove) override;

        void updateCheck() override;

        void updatePanel() override;

        void showPromotion() override;

    public:
        QtGame();

        ~QtGame() override;

        void updateBoard();

        void connectSignals(const view::Board& board) const;

    signals:

        QEvent* displayQtMessage(const QString& s);

        QEvent* updateGameQt(Coord selection[4],
                             std::array<std::array<TypePiece, xBoard>, yBoard>& board,
                             std::vector<Coord>& piecePossibleMove,
                             Color color, std::vector<TypePiece> deadPieces[2], int point);

        QEvent* updatePersonalizationQt(const std::array<std::array<TypePiece, xBoard>, yBoard>& board);

        QEvent* viewBoardQt(Color color);

        QEvent* updatePersonalizationMenu();

        QEvent* updatePiece();

        QEvent* addPieceQt(const TypePiece& typePiece, const Coord& pos);

        QEvent* movePieceQt(Coord& pos1, Coord& pos2);

        QEvent* killPieceQt(const Coord& pos);

        QEvent* updateSelectionQt(const Coord& pos, const std::vector<Coord>& piecePossibleMove);

        QEvent* updateCheckStateQt(const Coord& pos);

        QEvent* updatePanelQt(const std::vector<TypePiece> deadPieces[2], int point);

        QEvent* showPromotionQt(const Color& color);

    public slots:

        void doUpdateGame(Coord& coord);

        void doPromotePiece(const TypePiece& promotePiece);

        void doAddPiece(const TypePiece& typePiece, const Coord& pos);

        void doLoadGame();

        void doSwitchPlayer(const Color& color);

        void doSetDefaultBoard();

        void doResetBoard();

        void doSwitchRotation();
    };
}
#endif //CHESS_QTGAME_H
