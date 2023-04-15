#include <QMessageBox>
#include "Board.h"

namespace screen {

    Board::Board(CoordF tileSize, const std::string &resFile, Mode mode, QWidget *parent) :
    QGraphicsScene(parent), textureLoader_(), mode_(mode) {
        tileSize_ = tileSize;
        textureLoader_.setFileName(QString::fromStdString(resFile));
    }

    void Board::setLayer1(Coord selection[4]) {
        // Cases grises-blanches
        auto greyImg = getImage({6,1});

        auto whiteImg = getImage({6,0});

        auto selectedImg = getImage({7,1});

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
                auto img = getImage({(int) board[i][j].type, (int) board[i][j].color});
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

    void Board::updateGame(Coord selection[4], TypePiece boardGame[8][8],
                           std::vector<Coord> &piecePossibleMove, Color color) {
        mode_ = Mode::game;
        clear(); // Clear all items
        setLayer1(selection);
        setLayer2(boardGame);
        setPossibleMoves(piecePossibleMove);
        if (color != Color::none) {
            promoteColor_ = color;
            promote();
        }
    }

    void Board::updatePersonnalisation(TypePiece boardGame[8][8]) {
        mode_ = Mode::personalisation;
        this->clear(); // Clear all items
        setLayer1();
        setLayer2(boardGame);
        showPersonnalisationMenu();
    }

    void Board::setPossibleMoves(std::vector<Coord> &piecePossibleMove) {
        auto possibleMoveImg = getImage({7,0});
        for (Coord &possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::middle);
    }

    void Board::promote() {
        auto r = QColor::fromRgb(209, 207, 206);
        // afficher les 4 pièces
        std::array<std::pair<Type, Coord>, 4> promotablePieces = {
                std::pair<Type, Coord>{Type::queen, {3, 3}},
                std::pair<Type, Coord>{Type::rook, {4, 3}},
                std::pair<Type, Coord>{Type::bishop, {3, 4}},
                std::pair<Type, Coord>{Type::knight, {4, 4}}};
        QImage img;
        for (auto&& [promotablePiece, coord]: promotablePieces){
            img = getImage({(int) promotablePiece,(int) promoteColor_});
            drawRect(r, coord, ZLayer::top, true);
            addImage(img, coord, ZLayer::top, true);
        }
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

        switch (mode_) {
            case Mode::game:
                handleGameMode(pos);
                break;
            case Mode::personalisation:
                handlePersonnalisationMode(pos);
                break;
        }
    }


    void Board::handleGameMode(Coord& pos){
        if (side_ && selectedColor_ == Color::none)
            pos = {7 - pos.x, 7 - pos.y};

        if (selectedColor_ != Color::none){
            TypePiece type = getPieceToPromote(pos);
            if (type.type != Type::none) {
                emit promoteClicked(type, *this);
            } else
                return;
        }

        emit caseClicked(pos, *this);
    }

    TypePiece Board::getPieceToPromote(const Coord &pos) const {
        TypePiece type = {promoteColor_, Type::none};
        if (pos.x == 3 && pos.y == 3)
            type.type = Type::queen;
        else if (pos.x == 4 && pos.y == 3)
            type.type = Type::rook;
        else if (pos.x == 3 && pos.y == 4)
            type.type = Type::bishop;
        else if (pos.x == 4 && pos.y == 4)
            type.type = Type::knight;
        return type;
    }

    void Board::handlePersonnalisationMode(Coord &pos) {
        if (0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8) {
            if (side_)
                pos = {7 - pos.x, 7 - pos.y};
            emit pieceAdded(selectedPiece_, pos, *this);
        } else if (pos.x == 8 && 1 <= pos.y && pos.y < 7) {
            selectedPiece_ = {selectedColor_, (Type) (pos.y - 1)};
            selectedCoord_ = pos;
            emit caseClicked(pos, *this);
        } else if (pos.x == 8 && pos.y == 0) {
            selectedColor_ = selectedColor_ == Color::white ? Color::black : Color::white;
            selectedPiece_.color = selectedColor_;
            emit caseClicked(pos, *this);
        } else if (pos.x == 8 && pos.y == 7) {
            selectedPiece_ = {Color::none, Type::none};
            selectedCoord_ = pos;
            emit caseClicked(pos, *this);
        } else if (pos.x == 9 && pos.y == 0) {
            emit gameStarted(*this);
        } else if (pos.x == 9 && pos.y == 1) {
            // Reset board
            emit boardDefaulted(*this);
        } else if (pos.x == 9 && pos.y == 2) {
            emit boardReset(*this);
        } else if (pos.x == 9 && pos.y == 3) {
            side_ = !side_;
            emit playerSwitched(side_ ? Color::white : Color::black, *this);
        } else if (pos.x == 9 && pos.y == 4) {
            rotation = !rotation;
            emit rotationSwitched();
        }
    }

    void Board::viewBoard(Color color) {
        side_ = color == Color::white;
    }

    void Board::showPersonnalisationMenu() {
        for (int i = 0; i < 7; i++) {
            auto r = (selectedCoord_.y == i + 1) ? QColor::fromRgb(180, 150, 140) : QColor::fromRgb(209, 207, 206);
            drawRect(r,Coord{8, i + 1}, ZLayer::top, true);
            if (i == 6)
                continue;
            QImage img = getImage({i,(int)selectedColor_});
            addImage(img, Coord{8, i + 1}, ZLayer::top, true);
        }
        drawRect(QColor::fromRgb(70, 100, 130), {8, 0}, ZLayer::top, true);

        QImage img = getImage({6, (int) selectedColor_});
        int size = 17;
        img = img.scaled((int) tileSize_.x - size * 2, (int) tileSize_.y - size * 2, Qt::KeepAspectRatio);
        addImage(img, CoordF{8 + size / tileSize_.x, size / tileSize_.y}, ZLayer::top, true);
        drawRect(QColor::fromRgb(100, 70, 80),Coord{9, 0}, ZLayer::top, true, "Play");
        drawRect(QColor::fromRgb(180, 150, 140),Coord{9, 1}, ZLayer::top, true, "Default");
        drawRect(QColor::fromRgb(100, 200, 80),Coord{9, 2}, ZLayer::top, true, "Reset");
        drawRect(QColor::fromRgb(100, 70, 200),Coord{9, 3}, ZLayer::top, true, side_? "Set\nblack\nfirst" : "Set\nwhite\nfirst");
        drawRect(QColor::fromRgb(150, 20, 200),Coord{9, 4}, ZLayer::top, true, side_? "Disable\nrotation" : "Enable\nrotation");
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

    void Board::drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote, const std::string& text){
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), color);

        if (!text.empty()) {
            interface.drawText(interfaceImg.rect(), Qt::AlignCenter, QString::fromStdString(text));
        }

        addImage(interfaceImg, pos, zLayer, isPromote);
    }

}