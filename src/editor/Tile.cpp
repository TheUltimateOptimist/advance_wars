#include "Tile.hpp"
#include "SpriteProvider.hpp"
#include "EventBroker.hpp"

Tile::Tile(uint8_t id) : QGraphicsPixmapItem(SpriteProvider::get_sprite(0)), id(id), child(nullptr) {
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
    EventBroker::send([this](EventBroker* e){e->onTileEntered(this);}); 
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    EventBroker::send([this](EventBroker* e){e->onTileExited(this);}); 
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    EventBroker::send([this](EventBroker* e){e->onTileClicked(this);});
}
