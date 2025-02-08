#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

namespace advanced_wars
{

class Tile
{
    public:
        Tile(TileId id, int x, int y);

        void   render(Engine& engine, int scale);
        TileId getType() const;

    private:
        TileId m_id;
        int    m_x;
        int    m_y;
};

} // namespace advanced_wars