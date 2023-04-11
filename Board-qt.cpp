#include "Board-qt.h"
#include <iostream>

namespace screen {
    Board::Board(
            CoordF tileSize,
            const std::string& resFile,
            TypePiece board[8][8],
            QWidget* parent) : QGraphicsScene(parent), textureLoader_() {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
        Coord c[4] = {{-1, -1},{-1, -1},{-1, -1},{-1, -1}};
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
                auto pix = this->addPixmap(QPixmap::fromImage(img));
                pix->setZValue(static_cast<qreal>(ZLayer::middle));
                pix->setPos(
                        (float) i * tileSize_.x,
                        (float) j * tileSize_.y
                );
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
        auto r = QColor::fromRgb(224, 78, 74, 200);
        red.fillRect(0, 0, checkImg.width(), checkImg.height(), r);

        bool w = true;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                QImage img;
                bool imgSet = false;

                // Special images (Check&  selected)
                setSpecialCases(selection, selectedImg, checkImg, i, j, img, imgSet);

                if (!imgSet)
                  img = w ? whiteImg : greyImg;

                auto pix = this->addPixmap(QPixmap::fromImage(img));
                pix->setZValue(static_cast<qreal>(ZLayer::bottom));
                pix->setPos(
                        (float) i * tileSize_.x,
                        (float) j * tileSize_.y);
                w = !w;
            }
            w = !w;
        }
    }

    void Board::setSpecialCases(const Coord selection[4], const QImage& selectedImg, const QImage& checkImg, int i, int j,
                                QImage& img, bool& imgSet) const {
        for (int k = 0; k < 4; ++k) {
            if(selection[k].x == i && selection[k].y == j){
                if(k == 3)
                    img = checkImg;
                else
                    img = selectedImg;

                imgSet = true;
            }
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
        for (Coord& possibleCoord: piecePossibleMove) {
            auto pix = addPixmap(QPixmap::fromImage(possibleMoveImg));
            pix->setZValue(static_cast<qreal>(ZLayer::top));
            pix->setPos((float) possibleCoord.x * tileSize_.x, (float) possibleCoord.y * tileSize_.y);
        }
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        auto item = this->itemAt(event->scenePos(), QTransform());
        auto itemPos = item->pos();
        Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
        emit caseClicked(pos, *this);
    }

}