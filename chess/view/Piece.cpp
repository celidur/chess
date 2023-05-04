/**
* \file   Piece.cpp
* \author Charles Khoury et Gaëtan Florio
* \date   5 mai 2023
* Créé le 18 avril 2023
*/

#include "Piece.h"

view::Piece::Piece(QPointF pos, const QImage& image) {
    setPos(pos);
    setPixmap(QPixmap::fromImage(image));
    animation = nullptr;
}

void view::Piece::setPos(const QPointF& pos) {
    QGraphicsPixmapItem::setPos(pos);
}

QPointF view::Piece::getPos() {
    return QGraphicsPixmapItem::pos();
}

void view::Piece::move(const QPointF& pos) {
    animation = new QPropertyAnimation(this, "pos");
    animation->setStartValue(getPos());
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->setEndValue(pos);
    animation->start();
}
