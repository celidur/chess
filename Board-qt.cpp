#include "Board-qt.h"
#include <iostream>
#include <QImageReader>

namespace screen {
    Board::Board(CoordF tileSize, const std::string &resFile, TypePiece board[8][8], QWidget *parent) : QGraphicsScene(
            parent),
                                                                                                        boardLayers_() {
        boardLayers_.push_back(BoardMatrix());
        boardLayers_.push_back(BoardMatrix());
        tileSize_ = tileSize;
        setLayer1(resFile);
        setLayer2(resFile, board);
    }

    void Board::setLayer2(const std::string &resFile, TypePiece board[8][8]) {
        // Pièces
        auto &layer2 = boardLayers_[1];
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                if (board[i][j].type == Type::none)
                    continue;
                auto img = getPieceImg(
                        {
                                (int) tileSize_.x * (int) board[i][j].type,
                                (int) tileSize_.y * (int) board[i][j].color,
                                (int) tileSize_.x,
                                (int) tileSize_.y}, resFile);
                auto pix = this->addPixmap(QPixmap::fromImage(img));
                pix->setPos(
                        (float) i * tileSize_.x,
                        (float) j * tileSize_.y
                        );
                layer2.push_back(pix);
            }
        }
    }

    void Board::setLayer1(const std::string &resFile) {// Cases grises-blanches
        auto &layer1 = boardLayers_.front();
        auto greyImg = getPieceImg(
                {
                        (int) tileSize_.x * 6,
                        (int) tileSize_.y,
                        (int) tileSize_.x,
                        (int) tileSize_.y}, resFile);

        auto whiteImg = getPieceImg(
                {
                        (int) tileSize_.x * 6,
                        0,
                        (int) tileSize_.x,
                        (int) tileSize_.y}, resFile);


        bool w = true;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                auto img = w ? whiteImg : greyImg;
                auto pix = this->addPixmap(QPixmap::fromImage(img));
                pix->setPos(
                        (float) i * tileSize_.x,
                        (float) j * tileSize_.y);
                layer1.push_back(pix);
                w = !w;
            }
            w = !w;
        }
    }

    QImage Board::getPieceImg(const QRect &pieceRect, const std::string &resFile) {
        QImageReader imageReader;
        auto s = QString::fromStdString(resFile);
        imageReader.setFileName(s);
        imageReader.setClipRect(pieceRect);
        return imageReader.read();
    }

    void Board::update(Coord selection[4], TypePiece board_game[8][8], std::vector<Coord> &piecePossibleMove) {
        std::cout << "Board.update() called" << std::endl;



        //        for (int i = 0; i < 4; ++i) {
//            selection_[i] = selection[i];
//        }
//        layer3.clear();
//        layer3.setPrimitiveType(Quads);
//        layer3.resize(8 * 8 * 4);
//        layer2.clear();
//        layer2.setPrimitiveType(Quads);
//        layer2.resize(8 * 8 * 4);
//        for (auto &move: piecePossibleMove) {
//            int tv = 0;
//            int tu = 7;
//            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
//            int x = move.x;
//            int y = move.y;
//            Vertex *quad = &layer3[(x + y * 8) * 4];
//            // on définit ses quatre coins
//            quad[0].position = Vector2f(x * tile_size.x, y * tile_size.y);
//            quad[1].position = Vector2f((x + 1) * tile_size.x, y * tile_size.y);
//            quad[2].position = Vector2f((x + 1) * tile_size.x, (y + 1) * tile_size.y);
//            quad[3].position = Vector2f(x * tile_size.x, (y + 1) * tile_size.y);
//
//            // on définit ses quatre coordonnées de texture
//            quad[0].texCoords = Vector2f(tu * tile_size.x, tv * tile_size.y);
//            quad[1].texCoords = Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
//            quad[2].texCoords = Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
//            quad[3].texCoords = Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
//
//        }
//
//        for (int i = 0; i < 8; ++i) {
//            for (int j = 0; j < 8; ++j) {
//                if (board_game[i][j].type != Type::none) {
//                    int tv = (int) board_game[i][j].color;
//                    int tu = (int) board_game[i][j].type;
//                    // on récupère un pointeur vers le quad à définir dans le tableau de vertex
//                    Vertex *quad = &layer2[(i + j * 8) * 4];
//
//                    // on définit ses quatre coins
//                    quad[0].position = Vector2f(i * tile_size.x, j * tile_size.y);
//                    quad[1].position = Vector2f((i + 1) * tile_size.x, j * tile_size.y);
//                    quad[2].position = Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
//                    quad[3].position = Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
//
//                    // on définit ses quatre coordonnées de texture
//                    quad[0].texCoords = Vector2f(tu * tile_size.x, tv * tile_size.y);
//                    quad[1].texCoords = Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
//                    quad[2].texCoords = Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
//                    quad[3].texCoords = Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
//                }
//            }
//        }
    }

    void Board::applyToBoard(std::function<void(QGraphicsItem &)> functor) {
        std::for_each(boardLayers_.begin(), boardLayers_.end(), [&functor](BoardMatrix &b) {
            std::for_each(b.begin(), b.end(), [&functor](QGraphicsItem *item) {
                functor(*item);
            });
        });
    }

    void Board::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        auto item = this->itemAt(event->scenePos(), QTransform());
        auto itemPos = item->pos();
        Coord pos = {(int) itemPos.x() / (int) tileSize_.x, (int) itemPos.y() / (int) tileSize_.y};
        emit caseClicked(pos, *this);
    }

}