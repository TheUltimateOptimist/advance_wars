#pragma once

#include "Engine.hpp"

#include "Weapon.hpp"
#include <optional>
#include <unordered_map>

namespace advanced_wars
{

class Config;

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

    FIRST = INFANTERY,
    LAST = SUBMARINE
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
    WHEELED = 1,
    TREAD = 2,
    AIR = 3,
    SEA = 4,
    LANDER = 5,
};

using MatchupTable = std::unordered_map<UnitId, std::unordered_map<UnitId, int>>;

class Unit
{
    public:
        int m_x;
        int m_y;
        int m_health; // Current health of the unit, initialized to max health at construction.
        int m_price;

        int          m_movementPoints; // The number of tiles this unit can move per turn.
        MovementType m_movementType;   // The type of movement this unit has (e.g., foot, wheeled).

        /**
         * Constructor for Unit.
         * Initializes the unit's position, faction, identifier, state, and configuration settings.
         */
        Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state, Config& config);

        /**
         * Destructor for Unit.
         * Handles any cleanup necessary when a unit is destroyed.
         * (Currently assumes this triggers certain game events, though not explicitly detailed
         * here.)
         */
        ~Unit(){};

        /**
         * Renders the unit on the game screen.
         * Uses the engine's rendering capabilities to draw the unit based on its state and faction.
         *
         * @param engine The game engine responsible for rendering.
         * @param scale Scaling factor for rendering the unit.
         */
        void render(Engine& engine, int scale);

        /**
         * Determines if another unit (enemy) is within attack range.
         * Checks for the range based on the unit's weapon capabilities.
         *
         * @param enemy The enemy unit to check range against.
         * @return true if the enemy is in range, false otherwise.
         */
        bool inRange(Unit& enemy);

        /**
         * Initiates an attack on a specified enemy unit.
         * Calculates damage and updates health values for both the attacker and defender.
         * Attacker damages the enemy first; if the enemy survives, a counter-attack may occur.
         *
         * @param enemy The unit being attacked.
         */
        void attack(Unit& enemy);

        /**
         * Performs the calculated damage on a target unit, adjusting its health accordingly.
         * Considers this unit's current health for scaling damage.
         *
         * @param target The target unit receiving damage.
         * @param damage The amount of damage calculated to be applied.
         */
        void performAttack(Unit& target, int damage);

        /**
         * Calculates the potential damage this unit can inflict on a target.
         * Considers primary and secondary weapons' damage tables, and checks ammunition
         * availability.
         *
         * @param target The unit to calculate damage against.
         * @return The calculated damage value.
         */
        int calculateDamage(Unit& target);

        /**
         * Updates this unit's position on the game field.
         * Changes the internal position and recalculates the unit's state (e.g., direction it's
         * facing) based on movement to a new position.
         *
         * @param posX The new x-coordinate for the unit.
         * @param posY The new y-coordinate for the unit.
         */
        void updatePosition(int posX, int posY);

        /**
         * Determines potential movement paths based on the unit's movement type and current
         * terrain. (Intended to use Dijkstra's algorithm, though implementation details are omitted
         * here.)
         */
        void calculateMovement();

        /**
         * Recalculates and updates the unit's state based on movement direction.
         * Ensures that the unit faces the correct direction after a move.
         *
         * @param posX The x-coordinate of the desired position.
         * @param posY The y-coordinate of the desired position.
         */
        void calcState(int posX, int posY);

        /**
         * Processes a left-click event on this unit.
         * Typically triggers display of unit information (e.g., UI and movement range).
         *
         * @param event SDL event captured, specifically mouse click event.
         */
        void on_left_click(SDL_Event event);

        /**
         * Retrieves units within range that this unit can deal damage to.
         * Considers all units provided in 'allUnits', excluding itself, and checks movement and
         * weapon range.
         *
         * @param allUnits Vector of pointers to all units on the field to check against.
         * @return Vector of pointers to units in range that can be engaged.
         */
        std::vector<Unit*> getUnitsInRangeWithDamagePotential(const std::vector<Unit*>& allUnits);

        UnitFaction getFaction();

    private:
        UnitFaction m_faction; // The faction to which this unit belongs.
        UnitId      m_id;      // The identifier for the unit type.
        UnitState   m_state;   // The current state of the unit (idle, moving, etc.).

        int m_maxHealth; // The maximum health of the unit.
        int m_range;     // Possible range for future use, depending on specific unit abilities.

        bool m_hasMoved;    // Indicates whether the unit has moved this turn.
        bool m_hasAttacked; // Indicates whether the unit has attacked this turn.
        bool m_isSelected;  // Indicates whether the unit is currently selected.
        bool m_isTargeted;  // Indicates whether the unit is currently targeted by an enemy.

        Weapon m_secondaryWeapon; // The unit's secondary weapon.
        Weapon m_primaryWeapon;   // The unit's primary weapon.

        int m_cost;     // The cost associated with deploying this unit.
        int m_ammo;     // The amount of available ammo for attacks.
        int m_minRange; // The minimum range of the unit's attack capability.
        int m_maxRange; // The maximum range of the unit's attack capability.

        void renderHP(Engine& engine, int scale);
};
} // namespace advanced_wars