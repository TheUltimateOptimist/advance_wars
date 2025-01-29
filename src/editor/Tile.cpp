#include "Tile.hpp"
#include "SpriteProvider.hpp"

Tile::Tile(uint8_t id) : QGraphicsPixmapItem(SpriteProvider::get_sprite(0)), id(id) {
    this->setAcceptHoverEvents(true);
    this->setZValue(0);
    if (id > 0) {
        QPixmap pixmap = SpriteProvider::get_sprite(id);
        child = new QGraphicsPixmapItem(pixmap);
        child->setZValue(1);
        if (id >= 50) {
            child->setZValue(2);
        }
    }
}

uint8_t Tile::getId()
{
    return id;
}

uint8_t Tile::setId(uint8_t id)
{
    this->id = id;
}

QGraphicsPixmapItem *Tile::getChild()
{
    return child;
}

QGraphicsPixmapItem *Tile::setChild(QGraphicsPixmapItem *child)
{
    this->child = child;
}

void Tile::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    sendTileEntered(this);
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    sendTileExited(this);
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    sendTileClicked(this);
}
