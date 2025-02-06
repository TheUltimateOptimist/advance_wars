/**
 * Tile.hpp
 *
 * @author
 */

#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

namespace advanced_wars
{

/**
 * @brief Tile representation
 */
class Tile
{
    public:
        /**
         * Constructor
         *
         * @param id A tileId that describes this tiles type
         * @param x The tile-based x-position of this tile
         * @param y The tile-based y-position of this tile
         */
        Tile(TileId id, int x, int y);

        TileId m_id; // This tiles type
        int    m_x;  // This tiles x-position
        int    m_y;  // This tiles y-position

        /**
         * Renders the tile
         *
         * @param engine An engine object with valid rendering context
         * @param scale The factor to scale the rendering with
         */
        void   render(Engine& engine, int scale);
        TileId getType() const;
};

} // namespace advanced_wars