/**
* Tile.hpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QGraphicsPixmapItem>

namespace editor
{

class Tile : public QGraphicsPixmapItem {
public:
    Tile(int index, uint8_t id);

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    int m_index;
};

} // namespace editor