#pragma once

#include "engine.hpp"
#include "scene.hpp"

namespace advanced_wars
{

enum class TileId
{
    PLAIN = 0,
    WATER = 1,
    FOREST = 2,
    MOUNTAIN = 3,
    BRIDGE_HORIZONTAL = 4,
    BRIDGE_VERTICAL = 5,
    STREET_HORIZONTAL = 6,
    STREET_VERTICAL = 7,
    STREET_CROSSING = 8,
    STREET_JUNCTION_RIGHT = 9,
    STREET_JUNCTION_LEFT = 10,
    STREET_JUNCTION_DOWN = 11,
    STREET_JUNCTION_UP = 12,
    STREET_CORNER_TOP_LEFT = 13,
    STREET_CORNER_TOP_RIGHT = 14,
    STREET_CORNER_BOTTOM_LEFT = 15,
    STREET_CORNER_BOTTOM_RIGHT = 16,
    RIFF = 17,
    CLIFF_TOP = 18,
    CLIFF_BOTTOM = 19,
    CLIFF_LEFT = 20,
    CLIFF_RIGHT = 21,
    CLIFF_CORNER_TOP_LEFT = 22,
    CLIFF_CORNER_TOP_RIGHT = 23,
    CLIFF_CORNER_BOTTOM_LEFT = 24,
    CLIFF_CORNER_BOTTOM_RIGHT = 25,
    CLIFF_INVERSE_CORNER_TOP_LEFT = 26,
    CLIFF_INVERSE_CORNER_TOP_RIGHT = 27,
    CLIFF_INVERSE_CORNER_BOTTOM_LEFT = 28,
    CLIFF_INVERSE_CORNER_BOTTOM_RIGHT = 29,
};

class Tile
{
    public:
        Tile(TileId id, int x, int y);
        TileId m_id;
        int    m_x;
        int    m_y;

        void render(Engine& engine, int scale);
};

} // namespace advanced_wars