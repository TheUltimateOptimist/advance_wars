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

/**
 * The bottom (static) Layer of the LevelScene consists of Tile instances.
 * The purpose of the Tile object is to notify the QGraphicsScene when the mouse
 * enters/leaves/presses a specific tile of the map.
 * Thus Tile subclasses QGraphicsPixmapItem and overrides hoverEnterEvent, hoverLeaveEvent
 * and mousePressEvent.
 * Every Tile is instantiated with its index so that it can send that index to the LevelScene
 * in case it registers a hover/enter/press event.
 */
class Tile : public QGraphicsPixmapItem {
public:
    /**
     * Creates a Tile instance for the given index with the given tile id.
     * @param index The index of the tile if you were to linearize the level map.
     * @param id The tile id, e.g. 0 -> grass, 1-> water, ...
     */
    Tile(int index, uint8_t id);

private:
    /**
     * When a hover enter event is received it
     * emits the onTileEntered event method with the tile's index.
     * The LevelScene overwrites the onTileEntered event method.
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override;

    /**
     * When a hover leave event is received it
     * emits the onTileExited event method with the tile's index.
     * The LevelScene overwrites the onTileExited event method.
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;

    /**
     * When a mouse press event is received it
     * emits the onTileClicked event method with the tile's index.
     * The LevelScene overwrites the onTileClicked event method.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    /**
     * The index of the tile if you were to linearize the level map.
     */
    int m_index;
};

} // namespace editor