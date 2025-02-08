/**
 * Player.hpp
 *
 * @author Frederik Keens
 */

#pragma once

#include "Building.hpp"
#include "Unit.hpp"
#include <unordered_map>

namespace advanced_wars
{

class Player
{

    public:
        /**
         * Constructor
         *
         * @param money The players starting money
         * @param faction The faction this player will command
         */
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

        /**
         * @return This players faction
         */
        UnitFaction getFaction();

        /**
         * @return This players current money
         */
        int getMoney();

        /**
         * Decreses this players money by the given amount.
         * @param toSpend The money to subtract
         */
        void spendMoney(int toSpend);

    private:
        int         m_money;      // The players current amound of money
        bool        m_alive;      // Signals if the player is alive or not
        bool        m_activeTurn; // Signals if this player is currently the active player
        UnitFaction m_faction;    // The players commanding faction
};

} // namespace advanced_wars
