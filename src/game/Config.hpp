#pragma once

#include "Unit.hpp" // Include for UnitId and MovementType
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace advanced_wars
{
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
        explicit Config(const std::string& filename);

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
        int getUnitPrimaryWeaponDamage(UnitId attackerid, UnitId defenderid) const;

        /** @brief Retrieves the damage value of a unit's secondary weapon against a target unit
         * type. */
        int getUnitSecondaryWeaponDamage(UnitId attackerid, UnitId defenderid) const;

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
