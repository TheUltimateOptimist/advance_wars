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

        int             m_x;
        int             m_y;
        BuildingId      m_id;
        BuildingFaction m_faction;

        void render(Engine& engine, int scale);

        BuildingFaction getFaction();

        /**
        Changes the faction to the specified one

        @param faction The new faction the unit will belong to
        */
        void switch_faction(BuildingFaction faction);

        /*
        checks if the tile ontop of the building is free
        */
        bool check_spawn(std::unordered_map<int, advanced_wars::Unit>& units);

        /*
        checks if the player has enough money for the unit to be recruited
        */
        bool check_money(int price, int playerMoney);

        /*
        When the building is selected, the player should have the ability to recruit a selection of
        units They should be displayed by the UI On_click();
        */
        void recruit_unit();

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
};

} // namespace advanced_wars