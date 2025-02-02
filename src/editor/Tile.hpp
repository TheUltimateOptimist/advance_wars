#pragma once

#include <QGraphicsPixmapItem>

class Tile : public QGraphicsPixmapItem {
public:
    Tile(int index);

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    int index;
};