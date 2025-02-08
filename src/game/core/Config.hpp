#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <optional>
#include <string>
#include <unordered_map>

namespace advanced_wars
{
/* ENUMS FOR GLOBAL USE*/
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
    AIRPORT = 3,
    PORT = 4,
};

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

enum class TileId
{
    PLAIN = 0,
    WATER = 1,
    FOREST = 2,
    MOUNTAIN = 3,
    BRIDGE_HORIZONTAL = 4,
    BRIDGE_VERTICAL = 5,
    STREET_HORIZONTAL = 6,
    STREET_VERTICAL = 7,
    STREET_CROSSING = 8,
    STREET_JUNCTION_RIGHT = 9,
    STREET_JUNCTION_LEFT = 10,
    STREET_JUNCTION_DOWN = 11,
    STREET_JUNCTION_UP = 12,
    STREET_CORNER_TOP_LEFT = 13,
    STREET_CORNER_TOP_RIGHT = 14,
    STREET_CORNER_BOTTOM_LEFT = 15,
    STREET_CORNER_BOTTOM_RIGHT = 16,
    RIFF = 17,
    CLIFF_TOP = 18,
    CLIFF_BOTTOM = 19,
    CLIFF_LEFT = 20,
    CLIFF_RIGHT = 21,
    CLIFF_CORNER_TOP_LEFT = 22,
    CLIFF_CORNER_TOP_RIGHT = 23,
    CLIFF_CORNER_BOTTOM_LEFT = 24,
    CLIFF_CORNER_BOTTOM_RIGHT = 25,
    CLIFF_INVERSE_CORNER_TOP_LEFT = 26,
    CLIFF_INVERSE_CORNER_TOP_RIGHT = 27,
    CLIFF_INVERSE_CORNER_BOTTOM_LEFT = 28,
    CLIFF_INVERSE_CORNER_BOTTOM_RIGHT = 29,
};

/* END OF ALL ENUMS*/

/**
 * @class Config
 * @brief Parses and stores unit configuration data from an XML file.
 *
 * This class reads unit attributes, movement types, weapon data, and damage tables
 * from an XML configuration file and provides access to this data through getter methods.
 */
class Config
{
    public:
        /**
         * @brief Constructs a Config object and loads data from an XML file.
         *
         * The constructor reads the XML file, extracts unit properties, movement types,
         * and weapon damage tables, and stores them in internal data structures.
         *
         * @param filename Path to the XML configuration file.
         * @throws std::runtime_error if the file cannot be read or parsed.
         */
        Config(std::string filename);

        /** @brief Retrieves the cost of a given unit type. */
        int getUnitCost(UnitId id) const;

        /** @brief Retrieves the movement points of a given unit type. */
        int getUnitMovementPoints(UnitId id) const;

        /** @brief Retrieves the maximum ammunition capacity of a given unit type. */
        int getUnitAmmo(UnitId id) const;

        /** @brief Retrieves the minimum attack range of a given unit type. */
        int getUnitMinRange(UnitId id) const;

        /** @brief Retrieves the maximum attack range of a given unit type. */
        int getUnitMaxRange(UnitId id) const;

        /** @brief Retrieves the name of the primary weapon of a given unit type. */
        std::string getUnitPrimaryWeapon(UnitId id) const;

        /** @brief Retrieves the name of the secondary weapon of a given unit type. */
        std::string getUnitSecondaryWeapon(UnitId id) const;

        /** @brief Retrieves the damage value of a unit's primary weapon against a target unit type.
         */
        std::optional<int> getUnitPrimaryWeaponDamage(UnitId attackerid, UnitId defenderid) const;

        /** @brief Retrieves the damage value of a unit's secondary weapon against a target unit
         * type. */
        std::optional<int> getUnitSecondaryWeaponDamage(UnitId attackerid, UnitId defenderid) const;

        /** @brief Retrieves the movement type of a given unit type. */
        MovementType getUnitMovementType(UnitId id) const;

    private:
        /** @brief Maps unit IDs to their cost values. */
        std::unordered_map<UnitId, int> m_unitCosts;

        /** @brief Maps unit IDs to their movement points. */
        std::unordered_map<UnitId, int> m_unitMovementPoints;

        /** @brief Maps unit IDs to their maximum ammunition capacity. */
        std::unordered_map<UnitId, int> m_unitAmmo;

        /** @brief Maps unit IDs to their minimum attack range. */
        std::unordered_map<UnitId, int> m_unitMinRange;

        /** @brief Maps unit IDs to their maximum attack range. */
        std::unordered_map<UnitId, int> m_unitMaxRange;

        /** @brief Maps unit IDs to their primary weapon names. */
        std::unordered_map<UnitId, std::string> m_unitPrimaryWeapon;

        /** @brief Maps unit IDs to their secondary weapon names. */
        std::unordered_map<UnitId, std::string> m_unitSecondaryWeapon;

        /** @brief Stores primary weapon damage values for attacker-defender unit combinations. */
        std::unordered_map<UnitId, std::unordered_map<UnitId, int>> m_primaryWeaponDamage;

        /** @brief Stores secondary weapon damage values for attacker-defender unit combinations. */
        std::unordered_map<UnitId, std::unordered_map<UnitId, int>> m_secondaryWeaponDamage;

        /** @brief Maps unit IDs to their movement types. */
        std::unordered_map<UnitId, MovementType> m_unitMovementType;

        /**
         * @brief Converts a unit key string from the XML file to its corresponding UnitId.
         *
         * If the key is unknown, it returns UnitId::UNKNOWN.
         *
         * @param unit_key The string key representing a unit type.
         * @return The corresponding UnitId.
         */
        UnitId mapUnitKeyToID(const std::string& unit_key) const;

        /**
         * @brief Converts a movement type string from the XML file to its corresponding
         * MovementType.
         *
         * If the movement type is unknown, it returns MovementType::UNKNOWN.
         *
         * @param movementTypeStr The string representation of the movement type.
         * @return The corresponding MovementType.
         */
        MovementType mapMovementType(const std::string& movementTypeStr) const;
};
} // namespace advanced_wars
