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
            this->setZValue(2);
        }
    }
}

uint8_t Tile::getId()
{
    return id;
}

QGraphicsPixmapItem *Tile::getChild()
{
    return child;
}

void Tile::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    std::cout << "entered" << std::endl;
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
