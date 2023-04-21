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


namespace controller {
    class QtGame : public QObject, public logic::Game {
    Q_OBJECT

    protected:
        void displayMessage(const std::string& msg) override; // Addition to display the message in a system box

        void
        updateGameBoard(Coord selection[4], TypePiece boardGame[xBoard][yBoard], std::vector<Coord>& piecePossibleMove,
                        Color color, std::vector<TypePiece> deadPieces[2], int point);

        void viewBoard(Color color) override;

        void updatePersonalizationBoard(TypePiece boardGame[xBoard][yBoard]);

        void killPiece(Coord& pos) override;

        void movePiece(Coord& pos1, Coord& pos2) override;

        void updateSelection(Coord& pos, std::vector<Coord>& piecePossibleMove) override;

        void updateCheck() override;

        void updatePanel() override;

        void showPromotion() override;

    public:
        QtGame();

        ~QtGame() override;

        void updateBoard();

    signals:

        QEvent* displayQtMessage(const QString& s);

        QEvent* updateGameQt(Coord selection[4],
                             TypePiece boardGame[xBoard][yBoard],
                             std::vector<Coord>& piecePossibleMove,
                             Color color, std::vector<TypePiece> deadPieces[2], int point);

        QEvent* updatePersonalizationQt(TypePiece boardGame[xBoard][yBoard]);

        QEvent* viewBoardQt(Color color);

        QEvent* updatePersonalizationMenu();

        QEvent* updatePiece();

        QEvent* addPieceQt(TypePiece& typePiece, Coord& pos);

        QEvent* movePieceQt(Coord& pos1, Coord& pos2);

        QEvent* killPieceQt(Coord& pos);

        QEvent* updateSelectionQt(Coord& pos, std::vector<Coord>& piecePossibleMove);

        QEvent* updateCheckStateQt(Coord& pos);

        QEvent* updatePanelQt(std::vector<TypePiece> deadPieces[2], int point);

        QEvent* showPromotionQt(Color color);

    public slots:

        void doUpdateGame(Coord& coord);

        void doPromotePiece(TypePiece& promotePiece);

        void doAddPiece(TypePiece& typePiece, Coord& pos);

        void doLoadGame();

        void doSwitchPlayer(Color color);

        void doSetDefaultBoard();

        void doResetBoard();

        void doSwitchRotation();
    };
}
#endif //CHESS_QTGAME_H
