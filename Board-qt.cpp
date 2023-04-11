#include "Board-qt.h"

namespace screen {
    Board::Board(
            CoordF tileSize,
            const std::string& resFile,
            TypePiece board[8][8],
            QWidget* parent) : QGraphicsScene(parent), textureLoader_() {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
        Coord c[4] = {{-1, -1},
                      {-1, -1},
                      {-1, -1},
                      {-1, -1}};
        setLayer1(c);
        setLayer2(board);
    }

    void Board::setLayer2(TypePiece board[8][8]) {
        // Pièces
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j].type == Type::none)
                    continue;
                auto img = getPieceImg(
                        {
                                (int) tileSize_.x * (int) board[i][j].type,
                                (int) tileSize_.y * (int) board[i][j].color,
                                (int) tileSize_.x,
                                (int) tileSize_.y
                        });
                addImage(img, {i, j}, ZLayer::top);
            }
        }
    }

    void Board::setLayer1(Coord selection[4]) {
        // Cases grises-blanches
        auto greyImg = getPieceImg(
                {
                        (int) tileSize_.x * 6,
                        (int) tileSize_.y,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });

        auto whiteImg = getPieceImg(
                {
                        (int) tileSize_.x * 6,
                        0,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });

        auto selectedImg = getPieceImg(
                {
                        (int) tileSize_.x * 7,
                        (int) tileSize_.y,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });

        QImage checkImg((int) tileSize_.x, (int) tileSize_.y, QImage::Format::Format_ARGB32);
        QPainter red(&checkImg);
        auto r = QColor::fromRgb(224, 78, 74);
        red.fillRect(0, 0, checkImg.width(), checkImg.height(), r);

        bool w = true;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                addImage(w ? whiteImg : greyImg, {i, j}, ZLayer::bottom);
                for (int k = 3; k >= 0; --k) {
                    if (selection[k].x == i && selection[k].y == j) {
                        if (k == 3)
                            addImage(checkImg, {i, j}, ZLayer::bottom);
                        else
                            addImage(selectedImg, {i, j}, ZLayer::bottom);
                    }
                }
                w = !w;
            }
            w = !w;
        }
    }

    QImage Board::getPieceImg(const QRect& pieceRect) {
        if (textureLoader_.device()->isOpen())
            textureLoader_.device()->seek(0); // Reset the QImageReader

        textureLoader_.setClipRect(pieceRect);
        return textureLoader_.read();
    }

    void Board::update(Coord selection[4], TypePiece boardGame[8][8], std::vector<Coord>& piecePossibleMove) {
        this->clear(); // Clear all items
        setLayer1(selection);
        setLayer2(boardGame);
        setPossibleMoves(piecePossibleMove);
    }

    void Board::setPossibleMoves(std::vector<Coord>& piecePossibleMove) {
        auto possibleMoveImg = getPieceImg(
                {
                        (int) tileSize_.x * 7,
                        (int) 0,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        for (Coord &possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::top);
    }

    void Board::addImage(QImage& img, Coord coord, ZLayer zLayer) {
        if (side_)
            coord = {7 - coord.x, 7 - coord.y};
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) coord.x * tileSize_.x,
                (float) coord.y * tileSize_.y);
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        auto item = this->itemAt(event->scenePos(), QTransform());
        if (item == nullptr)
            return;

        auto itemPos = item->pos();
        Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
        if (side_)
            pos = {7 - pos.x, 7 - pos.y};
        emit caseClicked(pos, *this);
    }

}