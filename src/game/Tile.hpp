#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

namespace advanced_wars
{

class Tile
{
    public:
        Tile(TileId id, int x, int y);
        TileId m_id;
        int    m_x;
        int    m_y;

        void   render(Engine& engine, int scale);
        TileId getType() const;
};

} // namespace advanced_wars