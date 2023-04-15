/**
* \file   Bishop.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#ifndef BOARD
#define BOARD

#include "BoardBase.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QImageReader>
#include <qnamespace.h>
#include <QColor>
#include <QColorTransform>
#include <QPainter>
#include <memory>
#include <QMessageBox>

namespace screen {
    struct CoordF {
        float x, y;
    };

    enum class ZLayer {
        bottom,
        middle,
        top
    };


    class Board : public QGraphicsScene, public BoardBase {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, Mode mode, QWidget *parent = nullptr);

        ~Board() override = default;

        QImage getImage(Coord pos);

        void drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote, const std::string &text = "");

    signals:

        QEvent *caseClicked(Coord &coord, screen::Board &board);

        QEvent *pieceAdded(TypePiece &typePiece, Coord &pos, screen::Board &board);

        QEvent *gameStarted(screen::Board &board);

        QEvent *promoteClicked(TypePiece &, screen::Board &board);

        QEvent *boardReset(Board &board);

        QEvent *boardDefaulted(Board &board);

        QEvent *playerSwitched(Color color, Board &board);

        QEvent *rotationSwitched(screen::Board &board);

    public slots:

        static void displayMessage(const QString &s);

        void updateGame(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord> &piecePossibleMove,
                Color color,std::vector<TypePiece> deadPieces[2]) override;

        void viewBoard(Color color) override;

        void updatePersonalization(TypePiece boardGame[8][8]) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QImage getPieceImg(const QRect &pieceRect);

        void addImage(QImage &img, Coord coord, ZLayer zLayer, bool isPromote = false);

        void addImage(QImage &img, CoordF coord, ZLayer zLayer, bool isPromote = false);

        void setLayer1(Coord sel[4] = nullptr);

        void setLayer2(TypePiece board[8][8]);

        void showPanel(std::vector<TypePiece> deadPieces[2]);

        void showPersonalizationMenu();

        void promote();

        void setPossibleMoves(std::vector<Coord> &piecePossibleMove);

        void handleGameMode(Coord &pos);

        void handlePersonalizationMode(Coord &pos);

        [[nodiscard]] TypePiece getPieceToPromote(const Coord &pos) const;

        CoordF tileSize_;
        QImageReader textureLoader_;
        bool side_;
        TypePiece selectedPiece_;
        Color selectedColor_;
        Color promoteColor_;
        Mode mode_;
        bool rotation_;
    };

}

#endif // !BOARD
