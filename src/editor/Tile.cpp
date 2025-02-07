/**
* Tile.cpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "Tile.hpp"

#include "EventHandler.hpp"
#include "SpriteProvider.hpp"

namespace editor
{

Tile::Tile(int index, uint8_t id) : QGraphicsPixmapItem(SpriteProvider::get_sprite(id)), m_index(index)
{
    this->setAcceptHoverEvents(true);
}

void Tile::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    int index = this->m_index;
    EventHandler::send([index](EventHandler* e) { e->onTileEntered(index); });
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    int index = this->m_index;
    EventHandler::send([index](EventHandler* e) { e->onTileExited(index); });
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    int index = this->m_index;
    EventHandler::send([index](EventHandler* e) { e->onTileClicked(index); });
}

} // namespace editor