#include "Board-qt.h"

namespace screen {
    Board::Board(
            CoordF tileSize,
            const std::string &resFile,
            TypePiece board[8][8],
            QWidget *parent) : QGraphicsScene(parent), textureLoader_() {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
        Coord c[4] = {{-1, -1},
                      {-1, -1},
                      {-1, -1},
                      {-1, -1}};
        setLayer1(c);
        setLayer2(board);
        if (promoteColor_ != Colour::none) {
            promote();
        }
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
                addImage(img, {(float) i, (float) j}, ZLayer::middle);
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
                addImage(w ? whiteImg : greyImg, {(float) i, (float) j}, ZLayer::bottom);
                for (int k = 3; k >= 0; --k) {
                    if (selection[k].x == i && selection[k].y == j) {
                        if (k == 3)
                            addImage(checkImg, {(float) i, (float) j}, ZLayer::bottom);
                        else
                            addImage(selectedImg, {(float) i, (float) j}, ZLayer::bottom);
                    }
                }
                w = !w;
            }
            w = !w;
        }
    }

    QImage Board::getPieceImg(const QRect &pieceRect) {
        if (textureLoader_.device()->isOpen())
            textureLoader_.device()->seek(0); // Reset the QImageReader

        textureLoader_.setClipRect(pieceRect);
        return textureLoader_.read();
    }

    void Board::update(Coord selection[4], TypePiece boardGame[8][8],
                       std::vector<Coord> &piecePossibleMove, Colour color) {
        this->clear(); // Clear all items
        setLayer1(selection);
        setLayer2(boardGame);
        setPossibleMoves(piecePossibleMove);
        if (color != Colour::none) {
            promoteColor_ = color;
            promote();
        }
    }

    void Board::setPossibleMoves(std::vector<Coord> &piecePossibleMove) {
        auto possibleMoveImg = getPieceImg(
                {
                        (int) tileSize_.x * 7,
                        (int) 0,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        for (Coord &possibleCoord: piecePossibleMove) {
            CoordF coord = {(float) possibleCoord.x, (float) possibleCoord.y};
            addImage(possibleMoveImg, coord, ZLayer::middle);
        }
    }

    void Board::promote() {
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        auto r = QColor::fromRgb(209, 207, 206);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), r);
        // afficher les 4 pièces
        auto img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::queen,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        addImage(interfaceImg, {2, 3.5}, ZLayer::top, true);
        addImage(img, {2, 3.5}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::tower,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });

        addImage(interfaceImg, {3, 3.5}, ZLayer::top, true);
        addImage(img, {3, 3.5}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::bishop,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        addImage(interfaceImg, {4, 3.5}, ZLayer::top, true);
        addImage(img, {4, 3.5}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::knight,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        addImage(interfaceImg, {5, 3.5}, ZLayer::top, true);
        addImage(img, {5, 3.5}, ZLayer::top, true);
    }

    void Board::addImage(QImage &img, CoordF coord, ZLayer zLayer, bool isPromote) {
        if (side_ && !isPromote)
            coord = {7 - coord.x, 7 - coord.y};
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) coord.x * tileSize_.x,
                (float) coord.y * tileSize_.y);
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        auto item = this->itemAt(event->scenePos(), QTransform());
        if (item == nullptr)
            return;

        auto itemPos = item->pos();
        Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
        if (side_)
            pos = {7 - pos.x, 7 - pos.y};
        if (promoteColor_ == Colour::none)
                emit caseClicked(pos, *this);
        else {
            CoordF pos2 = {static_cast<float>(itemPos.x()) / tileSize_.x,
                           static_cast<float>(itemPos.y()) / tileSize_.y};
            TypePiece piece = {promoteColor_, Type::none};
            if (pos2.x == 2 && 3.5 == pos2.y)
                piece.type = Type::queen;
            else if (pos2.x == 3 && 3.5 == pos2.y)
                piece.type = Type::tower;
            else if (pos2.x == 4 && 3.5 == pos2.y)
                piece.type = Type::bishop;
            else if (pos2.x == 5 && 3.5 == pos2.y)
                piece.type = Type::knight;
            if (piece.type != Type::none) {
                 promoteColor_ = Colour::none;
                 emit promoteClicked(piece, *this);
            }
        }
    }

}