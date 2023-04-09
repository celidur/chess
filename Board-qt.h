#ifndef BOARD
#define    BOARD

#include "struct.h"
#include <BoardBase.h>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <qnamespace.h>
#include <QVector>
#include <memory>

namespace screen {

    using BoardMatrix = QVector<QGraphicsItem *>;


    class Board : public BoardBase, public QGraphicsScene {
    public:
        explicit Board(CoordF tileSize, const std::string &resFile, TypePiece board[8][8], QWidget *parent=nullptr);

        ~Board() override {
            auto deleteFunction = [](QGraphicsItem &item) {
                delete &item;
            };
            applyToBoard(deleteFunction);
        }

        void update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord> &piecePossibleMove) override;

    private:
        void draw() const;

        void applyToBoard(std::function<void(QGraphicsItem &)> funct);

        QVector<BoardMatrix> boardLayers_;

        static QImage getPieceImg(const QRect &pieceRect, const std::string &resFile);
    };

}

#endif // !BOARD
