/**
 * Building.hpp
 *
 * @author
 */

#pragma once

#include "Engine.hpp"
#include "Scene.hpp"
#include "Unit.hpp"
#include <unordered_map>

namespace advanced_wars
{

/**
 * Representation of a building
 */
class Building
{
    public:
        /**
         * Constructor
         *
         * @param x The tile-based x-position
         * @param y The tile-based y-position
         * @param id The building type
         * @param faction The faction of this building
         */
        Building(int x, int y, BuildingId id, BuildingFaction faction);

        /**
         * Render the building.Building
         *
         * @param engine An engine object with valid rendering context
         * @param scale The scaling factor for the current game
         */
        void render(Engine& engine, int scale);

        /**
         * X-Position getter
         *
         * @return tile-based x-position
         */
        int getXPosition();

        /**
         * Y-Position getter
         *
         * @return tile-based y-position
         */
        int getYPosition();

        /**
         * Faction getter
         *
         * @return This building's current faction
         */
        BuildingFaction getFaction();

        /**
         * ID getter
         *
         * @return This building's type
         */
        BuildingId getId();

        /**
        Changes the faction to the specified one

        @param faction The new faction the unit will belong to

        @return true if building was a headquarter
        */
        bool switch_faction(BuildingFaction faction);

        /*
        checks if the tile ontop of the building is free
        */
        bool check_spawn(std::unordered_map<int, advanced_wars::Unit>& units);

        /*
        checks if the player has enough money for the unit to be recruited
        */
        bool check_money(int price, int playerMoney);

        /**
        If the building is clicked, it shows information to the player, here it will be a list of
        all available units
         */
        void on_click();

        /**
         * Provides a vector of recruitable units, depending on the building id
         *
         */
        std::vector<UnitId> recruitableUnits();

    private:
        int             m_x;       // Tile-basded x-position
        int             m_y;       // Tile-based y-position
        BuildingId      m_id;      // Building's type
        BuildingFaction m_faction; // Building's faction
};

} // namespace advanced_wars