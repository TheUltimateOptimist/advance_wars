#include "Tile.hpp"
#include "SpriteProvider.hpp"
#include "EventBroker.hpp"

namespace editor
{

Tile::Tile(int index) : QGraphicsPixmapItem(SpriteProvider::get_sprite(0)), index(index) {
    this->setAcceptHoverEvents(true);
}

void Tile::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    int index = this->index;
    EventBroker::send([index](EventBroker* e){e->onTileEntered(index);}); 
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    int index = this->index;
    EventBroker::send([index](EventBroker* e){e->onTileExited(index);}); 
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int index = this->index;
    EventBroker::send([index](EventBroker* e){e->onTileClicked(index);});
}

} // namespace editor