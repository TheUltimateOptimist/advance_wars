#pragma once
#include <QGraphicsPixmapItem>

class Tile : public QGraphicsPixmapItem {
public:
    Tile(uint8_t id);
    uint8_t getId();
    uint8_t setId(uint8_t id);
    QGraphicsPixmapItem* getChild();
    QGraphicsPixmapItem* setChild(QGraphicsPixmapItem* child);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    uint8_t id;
    QGraphicsPixmapItem* child;
};