/**
* \file   Piece.h
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 18 avril 2023
*/
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "common/struct.h"

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

namespace view {
    class Piece : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ getPos WRITE setPos)
    public:
        Piece(QPointF pos, const QImage& image);

        void move(const QPointF& pos);

    public slots:

//        void setNewPos(const QPointF& pos);

        QPointF getPos();

    private:
        QPropertyAnimation *animation;
    };

}

#endif //CHESS_PIECE_H
