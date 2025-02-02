#pragma once

#include "Unit.hpp"

namespace advanced_wars
{

enum class PlayerFaction
{
    RED,
    BLUE,
    GREEN,
    YELLOW,
    PURPLE
};

class Player
{
    private:
        int           money;
        bool          alive;
        bool          activeTurn;
        PlayerFaction faction;

    public:
        Player(/* args */);
        ~Player();

        void startTurn();
        void endTurn();
};

} // namespace advanced_wars
