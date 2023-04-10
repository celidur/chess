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
#include <QVector>
#include <memory>

namespace screen {

    using BoardMatrix = QVector<QGraphicsItem*>;


    class Board :  public QGraphicsScene, public BoardBase{
        Q_OBJECT
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, TypePiece board[8][8], QWidget *parent=nullptr);

        ~Board() override {
            auto deleteFunction = [](QGraphicsItem& item){
                delete &item;
            };
            applyToBoard(deleteFunction);
        }

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove) override;

        signals:
        QEvent* caseClicked(Coord coord, screen::Board& board);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        void applyToBoard(std::function<void(QGraphicsItem &)> funct);

        QImage getPieceImg(const QRect& pieceRect);

        void setLayer1();

        void setLayer2(TypePiece board[8][8]);

        QVector<BoardMatrix> boardLayers_;
        QImageReader textureLoader_;

        inline static CoordF tileSize_ = {0,0};
    };

}

#endif // !BOARD
