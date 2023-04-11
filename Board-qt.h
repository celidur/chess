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

    enum class ZLayer {
        bottom,
        middle,
        top
    };

    class Board : public QGraphicsScene, public BoardBase {
    Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, TypePiece board[8][8], QWidget *parent = nullptr);

        ~Board() override = default;

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove,
                    Colour color = Colour::none) override;

    signals:

        QEvent *caseClicked(Coord coord, screen::Board &board);

        QEvent *promoteClicked(screen::TypePiece, screen::Board &board);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QImage getPieceImg(const QRect &pieceRect);

        void addImage(QImage &img, CoordF coord, ZLayer zLayer, bool isPromote = false);

        void setLayer1(Coord sel[4]);

        void setLayer2(TypePiece board[8][8]);

        void promote();

        void setPossibleMoves(std::vector<Coord> &piecePossibleMove);

        QImageReader textureLoader_;
        inline static CoordF tileSize_ = {0, 0};

        Colour promoteColor_ = Colour::none;
    };

}

#endif // !BOARD
