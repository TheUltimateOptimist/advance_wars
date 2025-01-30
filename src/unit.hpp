#pragma once

#include "engine.hpp"

namespace advanced_wars
{

enum class UnitFaction
{
    URED = 0,
    UBLUE = 1,
    UGREEN = 2,
    UYELLOW = 3,
    UPURPLE = 4,
};

enum class UnitId
{
    INFANTERY = 0,
    MECHANIZED_INFANTERY = 1,
    RECON = 2,
    MEDIUM_TANK = 3,
    HEAVY_TANK = 4,
    NEO_TANK = 5,
    APC = 6,
    ANTI_AIR_TANK = 7,
    ARTILLERY = 8,
    ROCKET_ARTILLERY = 9,
    ANTI_AIR_MISSILE_LAUNCHER = 10,
    FIGHTER = 11,
    BOMBER = 12,
    BATTLE_HELICOPTER = 13,
    TRANSPORT_HELICOPTER = 14,
    BATTLESHIP = 15,
    CRUISER = 16,
    LANDER = 17,
    SUBMARINE = 18,
};

enum class UnitState
{
    IDLE = 0,
    UNAVAILABLE = 1,
    MOVEMENTLEFT = 2,
    MOVEMENTRIGHT = 3,
    MOVEMENTDOWN = 4,
    MOVEMENTUP = 5,
};

enum class MovementType
{
    FOOT = 0,
    TIRES = 1,
    TREAD = 2,
    AIR = 3,
    SHIP = 4,
    LANDER = 5,
};

class Unit
{
    public:
        Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state);

        void render(Engine* engine, int scale);

    private:
        int         x;
        int         y;
        UnitFaction faction;
        UnitId      id;
        UnitState   state;
};

} // namespace advanced_wars