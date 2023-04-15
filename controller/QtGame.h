/**
* \file   QtGame.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
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
        void displayMessage(const std::string &msg) override; // Addition to display the message in a system box

        void updateGameBoard(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove,
                             Color color, std::vector<TypePiece> deadPieces[2]) override;

        void viewBoard(Color color) override;

        void updatePersonalizationBoard(TypePiece boardGame[8][8]) override;

    public:
        QtGame();

        explicit QtGame(const TypePiece board[8][8], Color color = Color::white);

        ~QtGame() override;

    signals:

        QEvent *displayQtMessage(const QString &s);

        QEvent *updateGameQt(Coord selection[4],
                             TypePiece boardGame[8][8],
                             std::vector<Coord> &piecePossibleMove,
                             Color color, std::vector<TypePiece> deadPieces[2]);

        QEvent *updatePersonalizationQt(TypePiece boardGame[8][8]);

        QEvent* viewBoardQt(Color color);

    public slots:

        void doUpdateGame(Coord &coord);

        void doPromotePiece(TypePiece &promotePiece);

        void doAddPiece(TypePiece &typePiece, Coord &pos);

        void doLoadGame();

        void doSwitchPlayer(Color color);

        void doSetDefaultBoard();

        void doResetBoard();

        void doSwitchRotation();
    };
}
#endif //CHESS_QTGAME_H
