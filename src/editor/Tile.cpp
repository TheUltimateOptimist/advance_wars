#include "Tile.hpp"
#include "SpriteProvider.hpp"
#include "EventBroker.hpp"

Tile::Tile(uint8_t id) : QGraphicsPixmapItem(SpriteProvider::get_sprite(0)), id(id), child(nullptr) {
    this->setAcceptHoverEvents(true);
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
