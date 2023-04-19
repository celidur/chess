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
#include <QCheckBox>
#include "Piece.h"

namespace view {
    enum class Sound {
        move,
        kill,
        check,
        none
    };

    enum class ZLayer {
        board,
        caseSelected,
        check,
        piece,
        move,
        top,
    };

    class Board : public QGraphicsScene, public BoardBase {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize,const std::string& resFile, Mode mode, QWidget* parent = nullptr);

        ~Board() override = default;

        QImage getImage(Coord pos);

        void drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote, const std::string& text = "");

    signals:

        QEvent* caseClicked(Coord& coord, view::Board& board);

        QEvent* pieceAdded(TypePiece& typePiece, Coord& pos, view::Board& board);

        QEvent* gameStarted(view::Board& board);

        QEvent* promoteClicked(TypePiece&, view::Board& board);

        QEvent* boardReset(Board& board);

        QEvent* boardDefaulted(Board& board);

        QEvent* playerSwitched(Color color, Board& board);

        QEvent* rotationSwitched(view::Board& board);

    public slots:

        static void displayMessage(const QString& s);

        void updateGame(
                Coord selection[4],
                TypePiece boardGame[8][8],
                std::vector<Coord>& piecePossibleMove,
                Color color, std::vector<TypePiece> deadPieces[2], int point) override;

        void updateDeadPieces(std::vector<TypePiece> deadPieces[2], int point) override;

        void movePiece(Coord& older,Coord& newer) override;

        void selectPiece(Coord& pos, std::vector<Coord>& piecePossibleMove) override;

        void viewBoard(Color color) override;

        void updatePersonalization(TypePiece boardGame[8][8]) override;

        void updatePersonalizationMenu() override;

        void updatePiece() override;

        void killPiece(Coord& pos) override;

        void addPiece(TypePiece& typePiece, Coord& pos) override;

        void updateCheckState(Coord& pos) override;

        void updatePanel(std::vector<TypePiece> deadPieces[2], int point) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    private:
        QImage getPieceImg(const QRect& pieceRect);

        QGraphicsPixmapItem* addImage(QImage& img, Coord coord, ZLayer zLayer, bool isPromote = false);

        QGraphicsPixmapItem* addImage(QImage& img, CoordF coord, ZLayer zLayer, bool isPromote = false);

        void setLayer1();

        void removeLayer(ZLayer zLayer);

        void setLayer2(TypePiece board[8][8]);

        void promote();

        QGraphicsPixmapItem* addCase(Coord pos, ZLayer zLayer);

        void handleGameMode(Coord& pos);

        void handlePersonalizationMode(Coord& pos);

        [[nodiscard]] TypePiece getPieceToPromote(const Coord& pos) const;

        std::unique_ptr<Piece> board_[8][8];
        QGraphicsPixmapItem* case_[4];
        QImageReader textureLoader_;
        bool side_;
        TypePiece selectedPiece_;
        Color selectedColor_;
        Color promoteColor_;
        Mode mode_;
        bool rotation_;
        Sound sound_;
        CoordF tileSize_;
    };

}

#endif // !BOARD
