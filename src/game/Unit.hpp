#pragma once

#include "Engine.hpp"
#include "Weapon.hpp"
#include <box2d/box2d.h>
#include <optional>
#include <unordered_map>

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
        int m_health; // health equals max_health at construction

        Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state, b2World* world);
        ~Unit()
        {
            // Assuming that the destruktion of a unit triggers events
        }

        void render(Engine& engine, int scale);

        /*
        Check if attacker is in Range to initiate combat
        TODO: This should probably tie back into rendering the units differently
        If a unit is selected, it should call inRange on all other enemy units on the field
        */

        bool inRange(Unit& enemy);

        /*
        The attacker will move towards the defender and thus initiate combat
        @params Takes a reference to the defender

        Will Update the health for both units
        Attacker deals damage to the defender first
        */

        void attack(Unit& enemy);

        /*
        @params Takes the desired position of the unit and updates its values
        This will teleport the unit, there is no smooth transition between tiles
        */
        void updatePosition(int posX, int posY);

        /*
        This function needs to be able to determine the possible movement-paths the unit can take
        MUST take into consideration that different units behave differently on certain terrain
        MUST show all movements possible
        */
        void calculateMovement();

        void calcState(int posX, int posY);

        /*
        This function will be called by an external event-handler, eventually.
        It should start displaying standard unit information, such as UI and move_range
        */
        void on_left_click(SDL_Event event);

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

    private:
        UnitFaction m_faction;
        UnitId      m_id;
        UnitState   m_state;
        b2Body*     m_body = nullptr;
        b2World*    m_world = nullptr;
        int         m_mapId = -1;

        int m_maxHealth; // max_health required for damage_scaling
        int m_range;
        int m_fuel;
        int m_maxFuel;

        bool m_hasMoved;
        bool m_hasAttacked;
        bool m_isSelected;
        bool m_isTargeted;

        Weapon m_secondaryWeapon;
        Weapon m_primaryWeapon;

        int m_ammo;
};

} // namespace advanced_wars