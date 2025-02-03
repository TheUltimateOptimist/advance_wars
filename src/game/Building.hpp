#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

namespace advanced_wars
{

enum class BuildingFaction
{
    RED = 0,
    BLUE = 1,
    YELLOW = 2,
    GREEN = 3,
    PURPLE = 4,
    NEUTRAL = 5,
};

enum class BuildingId
{
    HEADQUARTER = 0,
    CITY = 1,
    FACTORY = 2,
    PORT = 3,
    SATELLITE = 4,
};

class Building
{
    public:
        Building(int x, int y, BuildingId id, BuildingFaction faction);

        int             m_x;
        int             m_y;
        BuildingId      m_id;
        BuildingFaction m_faction;

        void render(Engine& engine, int scale);

        BuildingFaction getFaction();
};

} // namespace advanced_wars