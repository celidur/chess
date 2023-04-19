//
// Created by Gaetan on 2023-04-18.
//

#include "Piece.h"

view::Piece::Piece(QPointF pos, QImage image) {
    setPos(pos);
    setPixmap(QPixmap::fromImage(image));
}

void view::Piece::setPos(QPointF pos) {
    QGraphicsPixmapItem::setPos(pos);
}

QPointF view::Piece::getPos() {
    return QGraphicsPixmapItem::pos();
}

void view::Piece::move(QPointF pos) {
    animation = new QPropertyAnimation(this, "pos");
    animation->setStartValue(getPos());
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->setEndValue(pos);
    animation->start();
}
