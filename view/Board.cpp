#include <QMessageBox>
#include "Board.h"

namespace screen {

    Board::Board(CoordF tileSize, const std::string &resFile, QWidget *parent) :
    QGraphicsScene(parent), textureLoader_() {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
    }

    void Board::setLayer1(Coord selection[4]) {
        // Cases grises-blanches
        auto greyImg = getImage({6,1});

        auto whiteImg = getImage({6,0});

        auto selectedImg = getImage({7,0});

        QImage checkImg((int) tileSize_.x, (int) tileSize_.y, QImage::Format::Format_ARGB32);
        QPainter red(&checkImg);
        auto r = QColor::fromRgb(224, 78, 74);
        red.fillRect(0, 0, checkImg.width(), checkImg.height(), r);

        bool w = true;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                addImage(w ? whiteImg : greyImg, Coord{i, j}, ZLayer::bottom);
                w = !w;
                if (selection == nullptr)
                    continue;
                for (int k = 3; k >= 0; --k) {
                    if (selection[k].x == i && selection[k].y == j) {
                        if (k == 3)
                            addImage(checkImg, Coord{i, j}, ZLayer::bottom);
                        else
                            addImage(selectedImg, Coord{i, j}, ZLayer::bottom);
                    }
                }
            }
            w = !w;
        }
    }


    void Board::setLayer2(TypePiece board[8][8]) {
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
                addImage(img, Coord{i, j}, ZLayer::middle);
            }
        }
    }

    QImage Board::getPieceImg(const QRect &pieceRect) {
        if (textureLoader_.device()->isOpen())
            textureLoader_.device()->seek(0); // Reset the QImageReader

        textureLoader_.setClipRect(pieceRect);
        return textureLoader_.read();
    }

    void Board::update(Coord selection[4], TypePiece boardGame[8][8],
                       std::vector<Coord> &piecePossibleMove, Color color) {
        this->clear(); // Clear all items
        setLayer1(selection);
        setLayer2(boardGame);
        setPossibleMoves(piecePossibleMove);
        if (color != Color::none) {
            promoteColor_ = color;
            promote();
        }
    }

    void Board::update(TypePiece boardGame[8][8]) {
        this->clear(); // Clear all items
        setLayer1();
        setLayer2(boardGame);
        selectPiece();
    }

    void Board::setPossibleMoves(std::vector<Coord> &piecePossibleMove) {
        auto possibleMoveImg = getImage({7,0});
        for (Coord &possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::middle);
    }

    void Board::promote() {
        auto r = QColor::fromRgb(209, 207, 206);
        // afficher les 4 pièces
        auto img = getImage({(int) Type::queen,(int) promoteColor_});
        drawRect(r, Coord{3, 3}, ZLayer::top, true);
        addImage(img, Coord{3, 3}, ZLayer::top, true);
        img = getImage({(int) Type::rook,(int) promoteColor_});
        drawRect(r, Coord{4, 3}, ZLayer::top, true);
        addImage(img, Coord{4, 3}, ZLayer::top, true);
        img = getImage({(int) Type::bishop,(int) promoteColor_});
        drawRect(r, Coord{3, 4}, ZLayer::top, true);
        addImage(img, Coord{3, 4}, ZLayer::top, true);
        img = getImage({(int) Type::knight,(int) promoteColor_});
        drawRect(r, Coord{4, 4}, ZLayer::top, true);
        addImage(img, Coord{4, 4}, ZLayer::top, true);
    }

    void Board::addImage(QImage &img, Coord coord, ZLayer zLayer, bool isPromote) {
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
        emit caseClicked(pos, *this);
    }

    void Board::viewBoard(Color color) {
        side_ = color == Color::white;
    }

    void Board::selectPiece() {
        for (int i = 0; i < 7; i++) {
            auto r = (selectedCoord_.y == i + 1) ? QColor::fromRgb(180, 150, 140) : QColor::fromRgb(209, 207, 206);
            drawRect(r,Coord{8, i + 1}, ZLayer::top, true);
            if (i == 6)
                continue;
            QImage img = getImage({i,(int)selectedColor_});
            addImage(img, Coord{8, i + 1}, ZLayer::top, true);
        }
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        auto r = QColor::fromRgb(70, 100, 130);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), r);
        addImage(interfaceImg, Coord{8, 0}, ZLayer::top, true);
        QImage img = getImage({6, (int) selectedColor_});
        int size = 17;
        img = img.scaled((int) tileSize_.x - size * 2, (int) tileSize_.y - size * 2, Qt::KeepAspectRatio);
        addImage(img, CoordF{8 + size / tileSize_.x, size / tileSize_.y}, ZLayer::top, true);
        drawRect(QColor::fromRgb(100, 70, 80),Coord{9, 0}, ZLayer::top, true);
        drawRect(QColor::fromRgb(180, 150, 140),Coord{9, 1}, ZLayer::top, true);
        drawRect(QColor::fromRgb(100, 200, 80),Coord{9, 2}, ZLayer::top, true);
        drawRect(QColor::fromRgb(100, 70, 200),Coord{9, 3}, ZLayer::top, true);
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

    void Board::displayMessage(const QString& s) {
        QMessageBox msgBox;
        msgBox.setInformativeText(s);
        msgBox.exec();
    }

    QImage Board::getImage(Coord pos){
        return getPieceImg(
                {
                        (int) tileSize_.x * pos.x,
                        (int) tileSize_.y * pos.y,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
    }

    void Board::drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote){
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), color);
        addImage(interfaceImg, pos, zLayer, isPromote);
    }

}