#pragma once

#include "Engine.hpp"

#include "Weapon.hpp"
#include <SDL_events.h>
#include <box2d/box2d.h>
#include <optional>
#include <unordered_map>
#include <vector>

namespace advanced_wars
{

class Engine;
class Config;

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
        Unit(
            int x, int y, UnitFaction faction, UnitId id, UnitState state, Config& config,
            b2World* world = nullptr);

        /**
         * Destructor for Unit.
         * Handles any cleanup necessary when a unit is destroyed.
         * (Currently assumes this triggers certain game events, though not explicitly detailed
         * here.)
         */
        ~Unit() {};

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

        UnitFaction getFaction();

        void setState(UnitState state);

        /**
         * Retrieves units within range that this unit can deal damage to.
         * Considers all units provided in 'allUnits', excluding itself, and checks movement and
         * weapon range.
         *
         * @param allUnits Vector of pointers to all units on the field to check against.
         * @return Vector of pointers to units in range that can be engaged.
         */
        std::vector<Unit*> getUnitsInRangeWithDamagePotential(const std::vector<Unit*>& allUnits);

        /**
         * @brief Gibt die X-Koordinate der Unit im Tile-Raster zurück.
         *
         * Diese Methode liefert den Tile-Index in X-Richtung,
         * der angibt, in welcher Spalte die Unit positioniert ist.
         *
         * @return int Der X-Index (Tile-Koordinate).
         */
        int getX() const;

        /**
         * @brief Gibt die Y-Koordinate der Unit im Tile-Raster zurück.
         *
         * Diese Methode liefert den Tile-Index in Y-Richtung,
         * der angibt, in welcher Zeile die Unit positioniert ist.
         *
         * @return int Der Y-Index (Tile-Koordinate).
         */
        int getY() const;

        /**
         * @brief Gibt die aktuelle Gesundheit der Unit zurück.
         *
         * @return int Die Gesundheit der Unit.
         */
        int getHealth() const;

        /**
         * @brief Gibt die ID der Unit zurück.
         *
         * Diese ID ist möglicherweise nicht eindeutig, siehe @ref getMapId().
         *
         * @return int Die ID der Unit.
         */
        int getId() const;

        /**
         * @brief Setzt den Box2D-Body der Unit in der übergebenen Welt.
         *
         * Diese Methode erstellt den Box2D-Body und konfiguriert die Hitbox (als Sensor),
         * basierend auf der Position (Tile-Koordinaten) der Unit.
         *
         * @param world Zeiger auf die Box2D-Welt, in der der Body erstellt wird.
         */
        void setWorld(b2World* world);

        /**
         * @brief Berechnet und gibt die X-Position der Unit in Pixeln zurück.
         *
         * Dies ist hilfreich für das Rendering, um die Position in Pixelkoordinaten zu erhalten.
         *
         * @return float Die X-Position in Pixeln.
         */
        float getPixelX();

        /**
         * @brief Berechnet und gibt die Y-Position der Unit in Pixeln zurück.
         *
         * Dies ist hilfreich für das Rendering, um die Position in Pixelkoordinaten zu erhalten.
         *
         * @return float Die Y-Position in Pixeln.
         */
        float getPixelY();

        /**
         * @brief Setzt eine eindeutige Map-ID für die Unit.
         *
         * Diese eindeutige ID wird genutzt, um die Unit in der Level-Map
         * zu identifizieren, da die interne Unit-ID (getId()) möglicherweise nicht eindeutig ist.
         *
         * @param id Die eindeutige Map-ID, die der Unit zugewiesen werden soll.
         */
        void setMapId(int id);

        /**
         * @brief Gibt die eindeutige Map-ID der Unit zurück.
         *
         * Diese Map-ID ist eindeutig und entspricht dem Schlüssel, unter dem die Unit in der
         * Level-Map gespeichert ist.
         *
         * @return int Die eindeutige Map-ID der Unit.
         */
        int getMapId();

        /**
         * @brief Aktualisiert den Zustand der Unit.
         *
         * Diese Methode prüft, ob sich die Unit gerade bewegt und ob sie nahe genug am Ziel ist.
         * Ist das der Fall, wird die Bewegung gestoppt, und die Tile-Koordinaten werden
         * aktualisiert.
         */
        void update();

        void moveToTile(int targetX, int targetY);

    private:
        UnitFaction m_faction; // The faction to which this unit belongs.
        UnitId      m_id;      // The identifier for the unit type.
        UnitState   m_state;   // The current state of the unit (idle, moving, etc.).
        b2Body*     m_body = nullptr;
        b2World*    m_world = nullptr;
        int         m_mapId = -1;

        int m_targetTileX;
        int m_targetTileY;

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