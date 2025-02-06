#pragma once

#include "Building.hpp"
#include "Unit.hpp"
#include <unordered_map>

namespace advanced_wars
{

class Player
{
    private:
        int         m_money;
        bool        m_alive;
        bool        m_activeTurn;
        UnitFaction m_faction;

    public:
        Player(int money, UnitFaction faction);
        ~Player();

        /**
         * Sets all units of the players faction to idle, adds money for each building under the
         * players control and sets them as the active player.
         *
         * @param lvUnits All current units of a level
         * @param lvBuildings All buildings of a level
         */
        void startTurn(
            std::unordered_map<int, Unit>& lvUnits, std::unordered_map<int, Building>& lvBuildings);

        /**
         * Sets all units of the players faction to unavailable and sets them as no longer being the
         * active player.
         *
         * @param lvUnits All current units of a level
         */
        void endTurn(std::unordered_map<int, Unit>& lvUnits);

        UnitFaction getFaction();
};

} // namespace advanced_wars
