#pragma once
#include <QGraphicsPixmapItem>
#include "EventBroker.hpp"

class Tile : public QGraphicsPixmapItem, public EventBroker {
public:
    Tile(uint8_t id);
    uint8_t getId();
    QGraphicsPixmapItem* getChild();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    uint8_t id;
    QGraphicsPixmapItem* child;
};