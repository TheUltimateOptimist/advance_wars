#pragma once

#include "Engine.hpp"
#include "Scene.hpp"
#include "Unit.hpp"
#include <unordered_map>

namespace advanced_wars
{

class Building
{
    public:
        Building(int x, int y, BuildingId id, BuildingFaction faction);

        void render(Engine& engine, int scale);

        int xPosition();

        int yPosition();

        BuildingFaction getFaction();

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
        int             m_x;
        int             m_y;
        BuildingId      m_id;
        BuildingFaction m_faction;
};

} // namespace advanced_wars