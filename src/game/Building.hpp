#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

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

        /*
        If a unit moves onto a building, it will have the opportunity to capture the building, using an entire turn
        Interaction should be triggerd via the UI
        Checking if the capture is possible should be implemented in the unit.cpp
        If the Headquarter falls, the game should end in a victory for the corresponding player
        */
        void switch_allegiance(BuildingFaction faction);

        
        /*
        checks if the tile ontop of the building is free
        */
        bool check_spawn();

        /*
        checks if the player has enough money for the unit to be recruited
        */
        bool check_money(int price);

        /*
        When the building is selected, the player should have the ability to recruit a selection of units
        They should be displayed by the UI On_click();
        */
        void recruit_unit();

        void on_click();
};

} // namespace advanced_wars