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

    using BoardMatrix = QGraphicsPixmapItem*[8][8];

    enum class ZLayer{
        bottom,
        middle,
        top
    };

    class Board :  public QGraphicsScene, public BoardBase{
        Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string& resFile, TypePiece board[8][8], QWidget *parent=nullptr);

        ~Board() override {
            auto deleteFunction = [](QGraphicsItem* item){
                delete item;
            };
            applyToBoard(deleteFunction);
        }

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove) override;

        signals:
        QEvent* caseClicked(Coord coord, screen::Board& board);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        void applyToBoard(const std::function<void(QGraphicsItem *)>& functor);
        static void applyToLayer(BoardMatrix& layer, const std::function<void(QGraphicsItem *)>& functor);

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

        BoardMatrix layer1_{};
        BoardMatrix layer2_{};
        BoardMatrix layer3_{};
        QImageReader textureLoader_;
        inline static CoordF tileSize_ = {0,0};
    };

}

#endif // !BOARD
