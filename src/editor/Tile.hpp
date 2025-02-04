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
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    int m_index;
};

} // namespace editor