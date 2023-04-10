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

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove) override;


    private:
        void applyToBoard(std::function<void(QGraphicsItem &)> funct);

        static QImage getPieceImg(const QRect& pieceRect, const std::string &resFile) ;

        void setLayer1(const std::string &resFile);

        void setLayer2(const std::string &resFile, TypePiece board[8][8]);

        QVector<BoardMatrix> boardLayers_;

        inline static CoordF tileSize_ = {0,0};
    };

}

#endif // !BOARD
