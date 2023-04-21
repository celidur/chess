/**
* \file   Board.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2022
* Créé le 27 mars 2022
*/

#include <QtCore/QPropertyAnimation>
#include "Board.h"

namespace view {

    Board::Board(CoordF tileSize, const std::string& resFile, Mode mode, QWidget* parent) :
            tileSize_(tileSize),
            QGraphicsScene(parent),
            textureLoader_(),
            mode_(mode),
            side_(true),
            rotation_(false),
            selectedColor_(Color::white),
            promoteColor_(Color::none),
            sound_(Sound::none) {
        textureLoader_.setFileName(QString::fromStdString(resFile));
        resetBoard(board_);
        for (auto&& i : case_) {
            i = nullptr;
        }
    }

    void Board::resetBoard(std::unique_ptr<Piece> board[xBoard][yBoard]) {
        for (int i = 0; i < xBoard; ++i) {
            for (int j = 0; j < yBoard; ++j) {
                board[i][j] = nullptr;
            }
        }
    }

    void Board::setLayer1() {
        // Cases grises-blanches
        auto greyImg = getImage({6, 1});

        auto whiteImg = getImage({6, 0});

        bool w = true;
        for (int i = 0; i < xBoard; ++i) {
            bool tmp = w;
            for (int j = 0; j < yBoard; ++j) {
                addImage(w ? whiteImg : greyImg, Coord{i, j}, ZLayer::board);
                w = !w;
            }
            w = !tmp;
        }
    }


    void Board::setLayer2(TypePiece board[xBoard][yBoard]) {
        removeLayer(ZLayer::piece);
        for (int i = 0; i < xBoard; ++i) {
            for (int j = 0; j < yBoard; ++j) {
                if (board[i][j].type == Type::none) {
                    board_[i][j] = nullptr;
                    continue;
                }
                auto img = getImage({(int) board[i][j].type, (int) board[i][j].color});
                auto pos = Coord{i, j};
                if (side_)
                    pos = Coord{xBoard - 1 - i, yBoard - 1 - j};
                if (board_[i][j] != nullptr) {
                    board_[i][j] = nullptr;
                }
                board_[i][j] = std::make_unique<Piece>(
                        QPointF{(float) pos.x * tileSize_.x, (float) pos.y * tileSize_.y},
                        img);
                addItem(board_[i][j].get());
                board_[i][j]->setZValue(static_cast<qreal>(ZLayer::piece));
            }
        }
    }

    QImage Board::getPieceImg(const QRect& pieceRect) {
        if (textureLoader_.device()->isOpen())
            textureLoader_.device()->seek(0); // Reset the QImageReader

        textureLoader_.setClipRect(pieceRect);
        return textureLoader_.read();
    }

    void Board::updateGame(Coord selection[4], TypePiece boardGame[xBoard][yBoard],
                           std::vector<Coord>& piecePossibleMove, Color color, std::vector<TypePiece> deadPieces[2],
                           int point) {
        mode_ = Mode::game;
        for (auto&& i : case_) {
            i = nullptr;
        }
        resetBoard(board_);
        clear(); // Clear all items
        setLayer1();
        setLayer2(boardGame);
        selectPiece(selection[0], piecePossibleMove);
        updatePanel(deadPieces, point);
        if (color != Color::none) {
            promote(color);
        }

    }

    void Board::updatePersonalization(TypePiece boardGame[xBoard][yBoard]) {
        mode_ = Mode::personalisation;
        for (auto&& i : case_) {
            i = nullptr;
        }
        resetBoard(board_);
        this->clear(); // Clear all items
        setLayer1();
        setLayer2(boardGame);
        updatePersonalizationMenu();
    }

    void Board::selectPiece(Coord& pos, std::vector<Coord>& piecePossibleMove) {
        if (pos == Coord{-1, -1}) {
            if (case_[0] != nullptr)
                this->removeItem(case_[0]);
            case_[0] = nullptr;
            removeLayer(ZLayer::move);
            return;
        }
        selection_[0] = pos;
        if (case_[0] == nullptr) {
            auto selectedImg = getImage({7, 1});
            case_[0] = addImage(selectedImg, pos, ZLayer::caseSelected);
        } else {
            if (side_)
                pos = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
            case_[0]->setPos((float) pos.x * tileSize_.x, (float) pos.y * tileSize_.y);
        }
        removeLayer(ZLayer::move);
        auto possibleMoveImg = getImage({7, 0});
        for (Coord& possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::move);
    }

    void Board::promote(Color color) {
        promoteColor_ = color;
        auto r = QColor::fromRgb(209, 207, 206);
        // afficher les 4 pièces
        std::array<std::pair<Type, Coord>, 4> promotablePieces = {
                std::pair<Type, Coord>{Type::queen, {3, 3}},
                std::pair<Type, Coord>{Type::rook, {4, 3}},
                std::pair<Type, Coord>{Type::bishop, {3, 4}},
                std::pair<Type, Coord>{Type::knight, {4, 4}}};
        QImage img;
        for (auto&& [promotablePiece, coord]: promotablePieces) {
            img = getImage({(int) promotablePiece, (int) promoteColor_});
            drawRect(r, coord, ZLayer::promote, true);
            addImage(img, coord, ZLayer::promote, true);
        }
    }

    QGraphicsPixmapItem* Board::addImage(QImage& img, Coord coord, ZLayer zLayer, bool isPromote) {
        if (side_ && !isPromote)
            coord = {xBoard - 1 - coord.x, yBoard - 1 - coord.y};
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) coord.x * tileSize_.x,
                (float) coord.y * tileSize_.y);
        return pix;
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent* event) {
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
                handlePersonalizationMode(pos);
                break;
        }
    }


    void Board::handleGameMode(Coord& pos) {
        if (side_ && promoteColor_ == Color::none)
            pos = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
        if (promoteColor_ != Color::none) {
            TypePiece type = getPieceToPromote(pos);
            if (type.type != Type::none) {
                emit promoteClicked(type, *this);
                promoteColor_ = Color::none;
                removeLayer(ZLayer::promote);
            } else
                return;
        }
        emit caseClicked(pos, *this);
    }

    TypePiece Board::getPieceToPromote(const Coord& pos) const {
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

    void Board::handlePersonalizationMode(Coord& pos) {
        if (0 <= pos.x && pos.x < xBoard && 0 <= pos.y && pos.y < yBoard) {
            if (side_)
                pos = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
            emit pieceAdded(selectedPiece_, pos, *this);
        } else if (pos.x == xBoard && 1 <= pos.y && pos.y < 8) {
            selectedPiece_ = {selectedColor_, (Type) (pos.y - 1)};
            emit caseClicked(pos, *this);
        } else if (pos.x == xBoard && pos.y == 0) {
            selectedColor_ = selectedColor_ == Color::white ? Color::black : Color::white;
            selectedPiece_.color = selectedColor_;
            emit caseClicked(pos, *this);
        } else if (pos.x == xBoard + 1 && pos.y == 0) {
            emit gameStarted(*this);
        } else if (pos.x == xBoard + 1 && pos.y == 1) {
            emit boardDefaulted(*this);
        } else if (pos.x == xBoard + 1 && pos.y == 2) {
            emit boardReset(*this);
        } else if (pos.x == xBoard + 1 && pos.y == 3) {
            side_ = !side_;
            emit playerSwitched(side_ ? Color::white : Color::black, *this);
        } else if (pos.x == xBoard + 1 && pos.y == 4) {
            rotation_ = !rotation_;
            emit rotationSwitched(*this);
        }
    }

    void Board::viewBoard(Color color) {
        side_ = color == Color::white;
        updatePiece();
    }

    void Board::updatePersonalizationMenu() {
        removeLayer(ZLayer::top);
        for (int i = 0; i < 7; i++) {
            auto r = ((int) selectedPiece_.type == i) ?
                    QColor::fromRgb(180, 150, 140) : QColor::fromRgb(209, 207, 206);
            drawRect(r, Coord{xBoard, i + 1}, ZLayer::top, true);
            if (i == 6)
                continue;
            QImage img = getImage({i, (int) selectedColor_});
            addImage(img, Coord{xBoard, i + 1}, ZLayer::top, true);
        }
        drawRect(QColor::fromRgb(70, 100, 130), {xBoard, 0},
                 ZLayer::top, true);

        QImage img = getImage({6, (int) selectedColor_});
        float size = 17;
        img = img.scaled((int) (tileSize_.x - size * 2), (int) (tileSize_.y - size * 2),
                         Qt::KeepAspectRatio);
        addImage(img, CoordF{xBoard + size / tileSize_.x, size / tileSize_.y},
                 ZLayer::top, true);
        drawRect(
                QColor::fromRgb(100, 70, 80), Coord{xBoard + 1, 0},
                ZLayer::top, true, "Play");
        drawRect(QColor::fromRgb(180, 150, 140), Coord{xBoard + 1, 1},
                 ZLayer::top, true, "Default");
        drawRect(QColor::fromRgb(100, 200, 80), Coord{xBoard + 1, 2},
                 ZLayer::top, true, "Reset");
        drawRect(QColor::fromRgb(100, 70, 200), Coord{xBoard + 1, 3},
                 ZLayer::top, true,
                 side_ ? "Set\nblack\nfirst" : "Set\nwhite\nfirst");
        drawRect(QColor::fromRgb(150, 20, 200), Coord{xBoard + 1, 4},
                 ZLayer::top, true,
                 rotation_ ? "Disable\nrotation" : "Enable\nrotation");


        // place togle button
//        auto chechBox = new QCheckBox("Rotate");
//        chechBox->setStyleSheet("QCheckBox { color: white; }");
//        chechBox->setGeometry(0, 0, 100, 100);
//        chechBox->setChecked(rotation_);
        //set pos
//        auto p = this->addWidget(chechBox);
//        p->setZValue(static_cast<qreal>(ZLayer::top));
//        p->setPos(
//                (float) 9 * tileSize.x,
//                (float) 4 * tileSize.y);
    }

    QGraphicsPixmapItem* Board::addImage(QImage& img, CoordF coord, ZLayer zLayer, bool isPromote) {
        if (side_ && !isPromote)
            coord = {xBoard - 1 - coord.x, yBoard - 1 - coord.y};
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) coord.x * tileSize_.x,
                (float) coord.y * tileSize_.y);
        return pix;
    }

    void Board::displayMessage(const QString& s) {
        QMessageBox msgBox;
        msgBox.setInformativeText(s);
        msgBox.exec();
    }

    QImage Board::getImage(Coord pos) {
        return getPieceImg(
                {
                        (int) tileSize_.x * pos.x,
                        (int) tileSize_.y * pos.y,
                        (int) tileSize_.x,
                        (int) tileSize_.y
                });
    }

    void Board::drawRect(QColor color, Coord pos, ZLayer zLayer, bool isPromote, const std::string& text) {
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), color);

        if (!text.empty()) {
            interface.drawText(interfaceImg.rect(), Qt::AlignCenter, QString::fromStdString(text));
        }

        addImage(interfaceImg, pos, zLayer, isPromote);
    }

    void Board::updatePanel(std::vector<TypePiece> deadPieces[2], int point) {
        QImage interfaceImg((int) (tileSize_.x) * 2, (int) (tileSize_.y) * 8, QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(),
                           QColor::fromRgb(209, 207, 206));
        interface.drawText(QRectF{50, (float) (side_ ? 0 : 1) * 6 * tileSize_.y + 15,
                                  tileSize_.x * 2, tileSize_.y},Qt::AlignLeft, "player 2");
        interface.drawText(QRectF{50, (float) (side_ ? 1 : 0) * 6 * tileSize_.y + 15,
                                  tileSize_.x * 2, tileSize_.y},Qt::AlignLeft, "player 1");
        auto p = (point <= 0 ? "+" : "") + std::to_string(point * -1);
        interface.drawText(QRectF{110, (float) (side_ ? 0 : 1) * 6 * tileSize_.y + 15,
                                  tileSize_.x * 2, tileSize_.y},Qt::AlignLeft, p.c_str());
        p = (point >= 0 ? "+" : "") + std::to_string(point);
        interface.drawText(QRectF{110, (float) (side_ ? 1 : 0) * 6 * tileSize_.y + 15,
                                  tileSize_.x * 2, tileSize_.y},Qt::AlignLeft, p.c_str());
        addImage(interfaceImg, Coord{xBoard, 0}, ZLayer::top, true);
        float size = 17;
        QImage img = getImage({5, side_ ? 0 : 1})
                .scaled((int) (tileSize_.x - size * 2), (int) (tileSize_.y - size * 2),
                                                         Qt::KeepAspectRatio);
        addImage(img, CoordF{xBoard, 0}, ZLayer::top, true);
        img = getImage({5, side_ ? 1 : 0})
                .scaled((int) (tileSize_.x - size * 2), (int) (tileSize_.y - size * 2),
                                                  Qt::KeepAspectRatio);
        addImage(img, CoordF{xBoard, 6}, ZLayer::top, true);
        for (int i = 0; i < 2; ++i) {
            float x = 0;
            float y = 0;
            size = 26;
            for (auto&& piece: deadPieces[i]) {
                img = getImage({(int) piece.type, i}).scaled((int) (tileSize_.x - size * 2),
                                                             (int) (tileSize_.y - size * 2),
                                                             Qt::KeepAspectRatio);
                if (x == 8) {
                    x = 0;
                    y++;
                }
                if (y == 4)
                    break;
                addImage(img,CoordF{xBoard + (x * (size * 0.67f)) / tileSize_.x,
                                (float) ((i == 0 == side_ ? 1 : 0) * 6) + (y * size * 0.9f + 40) / tileSize_.y},
                         ZLayer::top,
                         true);
                x++;
            }

        }
    }

    void Board::updateDeadPieces(std::vector<TypePiece>* deadPieces, int point) {
        removeLayer(ZLayer::top);
        updatePanel(deadPieces, point);
    }

    void Board::movePiece(Coord& older, Coord& newer) {
        removeLayer(ZLayer::move);
        sound_ = board_[newer.x][newer.y] == nullptr ? Sound::move : Sound::kill;
        if (sound_ == Sound::kill) {
            this->removeItem(board_[newer.x][newer.y].get());
        }
        board_[newer.x][newer.y] = std::move(board_[older.x][older.y]);
        board_[older.x][older.y] = nullptr;
        auto piece = board_[newer.x][newer.y].get();
        selection_[1] = newer;
        selection_[2] = older;
        if (case_[1] != nullptr) {
            if (side_)
                newer = Coord{xBoard - 1 - newer.x, yBoard - 1 - newer.y};
            case_[1]->setPos((float) newer.x * tileSize_.x, (float) newer.y * tileSize_.y);
        } else {
            case_[1] = addCase(newer, ZLayer::caseSelected);
            if (side_)
                newer = Coord{xBoard - 1 - newer.x, yBoard - 1 - newer.y};
        }
        piece->move({(float) newer.x * tileSize_.x, (float) newer.y * tileSize_.y});
        if (case_[2] != nullptr) {
            if (side_)
                older = Coord{xBoard - 1 - older.x, yBoard - 1 - older.y};
            case_[2]->setPos((float) older.x * tileSize_.x, (float) older.y * tileSize_.y);
        } else {
            case_[2] = addCase(older, ZLayer::caseSelected);
        }
    }

    QGraphicsPixmapItem* Board::addCase(Coord pos, ZLayer zLayer) {
        auto selectedImg = getImage({7, 1});
        return addImage(selectedImg, pos, zLayer);
    }

    void Board::removeLayer(ZLayer zLayer) {
        for (auto& item: this->items()) {
            if (item->zValue() == static_cast<qreal>(zLayer)) {
                this->removeItem(item);
            }
        }
    }

    void Board::updatePiece() {
        for (int i = 0; i < 4; i++) {
            if (case_[i] != nullptr) {
                auto pos = side_ ? Coord{xBoard - 1 - selection_[i].x, yBoard - 1 - selection_[i].y}
                                  : selection_[i];
                case_[i]->setPos((float) pos.x * tileSize_.x, (float) pos.y * tileSize_.y);
            }
        }
        for (int i = 0; i < xBoard; ++i) {
            for (int j = 0; j < yBoard; ++j) {
                if (board_[i][j] != nullptr) {
                    auto pos = side_ ? Coord{xBoard - 1 - i, yBoard - 1 - j} : Coord{i, j};
                    board_[i][j]->move(QPointF((float) pos.x * tileSize_.x, (float) pos.y * tileSize_.y));
                }
            }
        }

    }

    void Board::addPiece(TypePiece& typePiece, Coord& pos) {
        auto img = getImage({(int) typePiece.type, (int) typePiece.color});
        if (board_[pos.x][pos.y] != nullptr) {
            this->removeItem(board_[pos.x][pos.y].get());
        }
        if (typePiece.type != Type::none) {
            auto em = side_ ? Coord{xBoard - 1 - pos.x, yBoard - 1 - pos.y} : pos;
            board_[pos.x][pos.y] = std::make_unique<Piece>(
                    QPointF{(float) em.x * tileSize_.x, (float) em.y * tileSize_.y}, img);
            addItem(board_[pos.x][pos.y].get());
            board_[pos.x][pos.y]->setZValue(static_cast<qreal>(ZLayer::piece));
        } else {
            board_[pos.x][pos.y] = nullptr;
        }
    }

    void Board::killPiece(Coord& pos) {
        if (board_[pos.x][pos.y] != nullptr) {
            this->removeItem(board_[pos.x][pos.y].get());
            board_[pos.x][pos.y] = nullptr;
        }
    }

    void Board::updateCheckState(Coord& pos) {
        if (pos == Coord{-1, -1}) {
            removeLayer(ZLayer::check);
            case_[3] = nullptr;
            return;
        }
        QImage checkImg((int) tileSize_.x, (int) tileSize_.y, QImage::Format::Format_ARGB32);
        QPainter red(&checkImg);
        auto r = QColor::fromRgb(224, 78, 74);
        red.fillRect(0, 0, checkImg.width(), checkImg.height(), r);
        selection_[3] = pos;
        case_[3] = addImage(checkImg, pos, ZLayer::check);
        case_[3]->setOpacity(0.85);
    }
}
