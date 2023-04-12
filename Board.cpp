#include "Board.h"

namespace screen {

    void resetBoard(TypePiece board[8][8]) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j].color = Color::none;
                board[i][j].type = Type::none;
            }
        }
    }

    void setDefaultBoard(TypePiece board[8][8]) {
        resetBoard(board);
        for (int i = 0; i < 8; ++i) {
            board[i][1] = {Color::white, Type::pawn};
            board[i][6] = {Color::black, Type::pawn};
        }
        board[0][0] = {Color::white, Type::rook};
        board[1][0] = {Color::white, Type::knight};
        board[2][0] = {Color::white, Type::bishop};
        board[3][0] = {Color::white, Type::king};
        board[4][0] = {Color::white, Type::queen};
        board[5][0] = {Color::white, Type::bishop};
        board[6][0] = {Color::white, Type::knight};
        board[7][0] = {Color::white, Type::rook};
        board[0][7] = {Color::black, Type::rook};
        board[1][7] = {Color::black, Type::knight};
        board[2][7] = {Color::black, Type::bishop};
        board[3][7] = {Color::black, Type::king};
        board[4][7] = {Color::black, Type::queen};
        board[5][7] = {Color::black, Type::bishop};
        board[6][7] = {Color::black, Type::knight};
        board[7][7] = {Color::black, Type::rook};
    }


    Board::Board(
            CoordF tileSize,
            const std::string &resFile,
            TypePiece board[8][8],
            QWidget *parent) : QGraphicsScene(parent), textureLoader_() {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
        if (mode_ == Mode::game) {
            setLayer1();
            setLayer2(board);
            if (promoteColor_ != Color::none) {
                promote();
            }
        } else if (mode_ == Mode::personalisation) {
            setDefaultBoard(board_);
            setLayer1();
            setLayer2(board_);
            selectPiece();
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
                addImage(img, Coord{i, j}, ZLayer::middle);
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

    void Board::setPossibleMoves(std::vector<Coord> &piecePossibleMove) {
        auto possibleMoveImg = getPieceImg(
                {
                        (int) tileSize_.x * 7,
                        (int) 0,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        for (Coord &possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::middle);
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
        addImage(interfaceImg, Coord{3, 3}, ZLayer::top, true);
        addImage(img, Coord{3, 3}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::rook,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });

        addImage(interfaceImg, Coord{4, 3}, ZLayer::top, true);
        addImage(img, Coord{4, 3}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::bishop,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        addImage(interfaceImg, Coord{3, 4}, ZLayer::top, true);
        addImage(img, Coord{3, 4}, ZLayer::top, true);
        img = getPieceImg(
                {
                        (int) tileSize_.x * (int) Type::knight,
                        (int) tileSize_.y * (int) promoteColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        addImage(interfaceImg, Coord{4, 4}, ZLayer::top, true);
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
        if (mode_ == Mode::game) {
            auto itemPos = item->pos();
            Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
            if (side_ && promoteColor_ == Color::none)
                pos = {7 - pos.x, 7 - pos.y};
            if (promoteColor_ == Color::none)
                    emit caseClicked(pos, *this);
            else {
                TypePiece piece = {promoteColor_, Type::none};
                if (pos.x == 3 && pos.y == 3)
                    piece.type = Type::queen;
                else if (pos.x == 4 && pos.y == 3)
                    piece.type = Type::rook;
                else if (pos.x == 3 && pos.y == 4)
                    piece.type = Type::bishop;
                else if (pos.x == 4 && pos.y == 4)
                    piece.type = Type::knight;
                if (piece.type != Type::none) {
                    promoteColor_ = Color::none;
                    emit promoteClicked(piece, *this);
                }
            }
        } else if (mode_ == Mode::personalisation) {
            auto itemPos = item->pos();
            Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
            if (0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8) {
                if (side_)
                    pos = {7 - pos.x, 7 - pos.y};
                board_[pos.x][pos.y] = selectedPiece_;
                this->clear(); // Clear all items
                setLayer1();
                setLayer2(board_);
                selectPiece();
            } else if (pos.x == 8 && 1 <= pos.y && pos.y < 7) {
                selectedPiece_ = {selectedColor_, (Type) (pos.y - 1)};
                selectedCoord_ = pos;
                selectPiece();
            } else if (pos.x == 8 && pos.y == 0) {
                selectedColor_ = selectedColor_ == Color::white ? Color::black : Color::white;
                selectedPiece_.color = selectedColor_;
                selectPiece();
            } else if (pos.x == 8 && pos.y == 7) {
                selectedPiece_ = {Color::none, Type::none};
                selectedCoord_ = pos;
                selectPiece();
            }

        }
    }

    void Board::viewBoard(Color color) {
        side_ = color == Color::white;
    }

    void Board::selectPiece() {
        for (int i = 0; i < 7; i++) {
            QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
            QPainter interface(&interfaceImg);
            auto r = (selectedCoord_.y == i + 1) ? QColor::fromRgb(180, 150, 140) : QColor::fromRgb(209, 207, 206);
            interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), r);
            addImage(interfaceImg, Coord{8, i + 1}, ZLayer::top, true);
            if (i == 6)
                continue;
            QImage img = getPieceImg(
                    {
                            (int) tileSize_.x * i,
                            (int) tileSize_.y * (int) selectedColor_,
                            (int) tileSize_.x,
                            (int) tileSize_.y
                    });
            addImage(img, Coord{8, i + 1}, ZLayer::top, true);
        }
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        auto r = QColor::fromRgb(70, 100, 130);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), r);
        addImage(interfaceImg, Coord{8, 0}, ZLayer::top, true);
        QImage img = getPieceImg(
                {
                        (int) tileSize_.x * 6,
                        (int) tileSize_.y * (int) selectedColor_,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
        int size = 17;
        img = img.scaled((int) tileSize_.x - size * 2, (int) tileSize_.y - size * 2, Qt::KeepAspectRatio);
        addImage(img, CoordF{8 + size / tileSize_.x, size / tileSize_.y}, ZLayer::top, true);

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

}