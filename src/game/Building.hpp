#pragma once

#include <unordered_map>
#include "Engine.hpp"
#include "Scene.hpp"
#include "Unit.hpp"

namespace advanced_wars
{

enum class BuildingFaction
{
    URED = 0,
    UBLUE = 1,
    UGREEN = 2,
    UYELLOW = 3,
    UPURPLE = 4,
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
        bool check_money(int price);

        /*
        When the building is selected, the player should have the ability to recruit a selection of units
        They should be displayed by the UI On_click();
        */
        void recruit_unit();

        /**
        If the building is clicked, it shows information to the player, here it will be a list of all available units
         */
        void on_click();
};

} // namespace advanced_wars