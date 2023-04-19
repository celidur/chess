//
// Created by Gaetan on 2023-04-18.
//
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
        Piece(QPointF pos, QImage image);

        void move(QPointF pos);

    public slots:

        void setPos(QPointF pos);

        QPointF getPos();

    private:
        QPropertyAnimation *animation;
    };

}

#endif //CHESS_PIECE_H
