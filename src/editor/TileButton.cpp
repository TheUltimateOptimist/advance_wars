/**
* TileButton.cpp
*
* @date 29.01.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) minor changes
*/

#include "TileButton.hpp"

#include "EventHandler.hpp"
#include "SpriteProvider.hpp"

namespace editor
{

TileButton::TileButton(const uint8_t id, QWidget* parent) : QPushButton(parent), m_id(id)
{
    QPixmap pixmap = SpriteProvider::get_sprite(id);
    QPixmap scaledpixmap = pixmap.scaled(72, 72, Qt::KeepAspectRatio, Qt::FastTransformation);
    this->setIcon(QIcon(scaledpixmap));
    this->setIconSize(QSize(72, 72));
    this->setFixedSize(80, 80);
}

void TileButton::mousePressEvent(QMouseEvent* event)
{
    QPushButton::mousePressEvent(event);
    uint8_t tile_id = m_id;
    EventHandler::send([tile_id](EventHandler* e) { e->onNewTileIdSelected(tile_id); });
}

} // namespace editor