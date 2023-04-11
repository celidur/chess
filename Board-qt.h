#ifndef BOARD
#define    BOARD

#include "struct.h"
#include <BoardBase.h>
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

namespace screen {

    enum class ZLayer{
        bottom,
        middle,
        top
    };

    class Board :  public QGraphicsScene, public BoardBase{
        Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string& resFile, TypePiece board[8][8], QWidget *parent=nullptr);

        ~Board() override = default;

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove) override;

        signals:
        QEvent* caseClicked(Coord coord, screen::Board& board);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QImage getPieceImg(const QRect& pieceRect);

        void setLayer1(Coord sel[4]);

        void setLayer2(TypePiece board[8][8]);

        void setSpecialCases(
                const Coord selection[4],
                const QImage& selectedImg,
                const QImage& checkImg,
                int i, int j,
                QImage& img,
                bool& imgSet) const;

        void setPossibleMoves(std::vector<Coord>& piecePossibleMove);

        QImageReader textureLoader_;
        inline static CoordF tileSize_ = {0,0};
    };

}

#endif // !BOARD
