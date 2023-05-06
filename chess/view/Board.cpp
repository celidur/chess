/**
* \file   Board.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 27 mars 2023
*/

#include <QtCore/QPropertyAnimation>
#include "Board.h"

namespace view {

    Board::Board(const CoordF tileSize, const std::string& resFile, Mode mode, QWidget* parent) :
            IBoard(tileSize, resFile, parent) {
        mode_ = StateFactory::getModeInstance(mode);
        textureLoader_.setFileName(QString::fromStdString(resFile));
        resetBoard();
        for (auto&& i: case_) {
            i = nullptr;
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
                addImage(w ? whiteImg : greyImg, Coord{i, j}, ZLayer::board, false);
                w = !w;
            }
            w = !tmp;
        }
    }


    void Board::setLayer2(const std::array<std::array<TypePiece, xBoard>, yBoard>& board) {
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

    void Board::updateGame(const Coord selection[4], const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame,
                           const std::vector<Coord>& piecePossibleMove, const Color& color,
                           const std::vector<TypePiece> deadPieces[2],
                           const int point) {
        mode_ = StateFactory::getModeInstance(Mode::game);
        mode_->handleUpdateEvent(*this, selection, boardGame, piecePossibleMove, color, deadPieces, point);
    }

    void Board::updatePersonalization(const std::array<std::array<TypePiece, xBoard>, yBoard>& boardGame) {
        mode_ = StateFactory::getModeInstance(Mode::personalisation);
        mode_->handleUpdateEvent(*this, boardGame);
    }

    void Board::selectPiece(const Coord& pos, const std::vector<Coord>& piecePossibleMove) {
        Coord selection;
        if (pos == selection) {
            if (case_[0] != nullptr)
                this->removeItem(case_[0]);
            case_[0] = nullptr;
            removeLayer(ZLayer::move);
            return;
        }
        selection_[0] = pos;
        if (case_[0] == nullptr) {
            auto selectedImg = getImage({7, 1});
            case_[0] = addImage(selectedImg, pos, ZLayer::caseSelected, false);
        } else {
            if (side_)
                selection = {xBoard - 1 - pos.x, yBoard - 1 - pos.y};
            case_[0]->setPos((float) selection.x * tileSize_.x, (float) selection.y * tileSize_.y);
        }
        removeLayer(ZLayer::move);
        auto possibleMoveImg = getImage({7, 0});
        for (auto& possibleCoord: piecePossibleMove)
            addImage(possibleMoveImg, possibleCoord, ZLayer::move, false);
    }

    void Board::promote(const Color& color) {
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
            drawRect(r, coord, ZLayer::promote, true, "");
            addImage(img, coord, ZLayer::promote, true);
        }
    }

    QGraphicsPixmapItem*
    Board::addImage(const QImage& img, const Coord& coord, const ZLayer& zLayer, const bool isPromote) {
        Coord pos = (side_ && !isPromote) ? Coord{xBoard - 1 - coord.x, yBoard - 1 - coord.y} : coord;
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) pos.x * tileSize_.x,
                (float) pos.y * tileSize_.y);
        return pix;
    }

    QGraphicsPixmapItem*
    Board::addImage(const QImage& img, const CoordF& coord, const ZLayer& zLayer, const bool isPromote) {
        CoordF pos = (side_ && !isPromote) ?
                     CoordF{xBoard - 1 - coord.x, yBoard - 1 - coord.y} : coord;
        auto pix = this->addPixmap(QPixmap::fromImage(img));
        pix->setZValue(static_cast<qreal>(zLayer));
        pix->setPos(
                (float) pos.x * tileSize_.x,
                (float) pos.y * tileSize_.y);
        return pix;
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        auto item = this->itemAt(event->scenePos(), QTransform());
        if (item == nullptr)
            return;
        auto itemPos = item->pos();
        Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};

        mode_->handleMouseEvent(*this, pos);
    }

    void Board::viewBoard(const Color& color) {
        side_ = color == Color::white;
        updatePiece();
    }

    void Board::displayMessage(const QString& s) {
        QMessageBox msgBox;
        msgBox.setInformativeText(s);
        msgBox.exec();
    }

    QImage Board::getImage(const Coord& pos) {
        if (textureLoader_.device()->isOpen())
            textureLoader_.device()->seek(0); // Reset the QImageReader
        textureLoader_.setClipRect(
                {(int) tileSize_.x * pos.x, (int) tileSize_.y * pos.y, (int) tileSize_.x, (int) tileSize_.y});
        return textureLoader_.read();
    }

    void Board::drawRect(const QColor& color, const Coord& pos, const ZLayer& zLayer, const bool isPromote,
                         const std::string& text) {
        QImage interfaceImg((int) (tileSize_.x), (int) (tileSize_.y), QImage::Format::Format_ARGB32);
        QPainter interface(&interfaceImg);
        interface.fillRect(0, 0, interfaceImg.width(), interfaceImg.height(), color);
        if (!text.empty()) {
            interface.drawText(interfaceImg.rect(), Qt::AlignCenter, QString::fromStdString(text));
        }
        addImage(interfaceImg, pos, zLayer, isPromote);
    }


    void Board::updateDeadPieces(const std::vector<TypePiece>* deadPieces, const int point) {
        removeLayer(ZLayer::top);
        mode_->handleUpdatePanel(*this, deadPieces, point);
    }

    void Board::movePiece(Coord& older, Coord& newer) {
        removeLayer(ZLayer::move);
        if (board_[newer.x][newer.y] != nullptr) {
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

    QGraphicsPixmapItem* Board::addCase(const Coord& pos, const ZLayer& zLayer) {
        auto selectedImg = getImage({7, 1});
        return addImage(selectedImg, pos, zLayer, false);
    }

    void Board::removeLayer(const ZLayer& zLayer) {
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

    void Board::addPiece(const TypePiece& typePiece, const Coord& pos) {
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

    void Board::killPiece(const Coord& pos) {
        if (board_[pos.x][pos.y] != nullptr) {
            this->removeItem(board_[pos.x][pos.y].get());
            board_[pos.x][pos.y] = nullptr;
        }
    }

    void Board::updateCheckState(const Coord& pos) {
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
        case_[3] = addImage(checkImg, pos, ZLayer::check, false);
        case_[3]->setOpacity(0.85);
    }

    void Board::updatePanel(const std::vector<TypePiece> *deadPieces, int point) {
        mode_->handleUpdatePanel(*this, deadPieces, point);
    }

    void Board::updatePanelP() {
        mode_->handleUpdatePanel(*this);
    }
}
