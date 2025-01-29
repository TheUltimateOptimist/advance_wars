#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "unit.hpp" // Include für UnitId

namespace advanced_wars
{
/**
     * @class Config
     * @brief This class loads and stores configuration data for different unit types.
     * 
     * The Config class parses an XML configuration file to extract unit attributes such as cost, movement points,
     * ammunition, attack ranges, and weapon data. It provides getter functions to retrieve unit properties based
     * on their unique identifiers.
     */
    class Config
    {
    public:
        /**
         * @brief Constructor that loads configuration from an XML file.
         * @param filename The path to the XML configuration file.
         * 
         * The constructor reads the XML file using Boost.PropertyTree and extracts relevant unit information.
         * It initializes multiple unordered_maps to store unit attributes and weapon damage tables.
         */
        explicit Config(const std::string &filename);

        /**
         * @brief Retrieves the cost of a given unit.
         * @param id The unique identifier of the unit.
         * @return The cost of the unit.
         * @throws std::runtime_error if the unit ID is not found.
         */
        int get_unit_cost(UnitId id) const;

        /**
         * @brief Retrieves the movement points of a given unit.
         * @param id The unique identifier of the unit.
         * @return The number of movement points.
         * @throws std::runtime_error if the unit ID is not found.
         */
        int get_unit_movement_points(UnitId id) const;

        /**
         * @brief Retrieves the ammunition capacity of a given unit.
         * @param id The unique identifier of the unit.
         * @return The ammunition count.
         * @throws std::runtime_error if the unit ID is not found.
         */
        int get_unit_ammo(UnitId id) const;

        /**
         * @brief Retrieves the minimum attack range of a given unit.
         * @param id The unique identifier of the unit.
         * @return The minimum attack range.
         * @throws std::runtime_error if the unit ID is not found.
         */
        int get_unit_min_range(UnitId id) const;

        /**
         * @brief Retrieves the maximum attack range of a given unit.
         * @param id The unique identifier of the unit.
         * @return The maximum attack range.
         * @throws std::runtime_error if the unit ID is not found.
         */
        int get_unit_max_range(UnitId id) const;

        /**
         * @brief Retrieves the primary weapon name of a given unit.
         * @param id The unique identifier of the unit.
         * @return The name of the primary weapon.
         * @throws std::runtime_error if the unit ID is not found.
         */
        std::string get_unit_primaryweapon(UnitId id) const;

        /**
         * @brief Retrieves the secondary weapon name of a given unit.
         * @param id The unique identifier of the unit.
         * @return The name of the secondary weapon.
         * @throws std::runtime_error if the unit ID is not found.
         */
        std::string get_unit_secondaryweapon(UnitId id) const;

        /**
         * @brief Retrieves the damage value of a primary weapon when attacking another unit.
         * @param attackerid The unit ID of the attacker.
         * @param defenderid The unit ID of the defender.
         * @return The damage value inflicted by the primary weapon.
         * @throws std::runtime_error if the damage value is not found for the given attacker/defender combination.
         */
        int get_unit_primaryweapon_damage(UnitId attackerid, UnitId defenderid) const;

        /**
         * @brief Retrieves the damage value of a secondary weapon when attacking another unit.
         * @param attackerid The unit ID of the attacker.
         * @param defenderid The unit ID of the defender.
         * @return The damage value inflicted by the secondary weapon.
         * @throws std::runtime_error if the damage value is not found for the given attacker/defender combination.
         */
        int get_unit_secondaryweapon_damage(UnitId attackerid, UnitId defenderid) const;

    private:
        // Maps to store unit attributes
        std::unordered_map<UnitId, int> unit_costs;                 ///< Stores the cost of each unit type.
        std::unordered_map<UnitId, int> unit_movement_points;       ///< Stores the movement points of each unit type.
        std::unordered_map<UnitId, int> unit_ammo;                  ///< Stores the ammunition capacity of each unit type.
        std::unordered_map<UnitId, int> unit_min_range;             ///< Stores the minimum attack range of each unit type.
        std::unordered_map<UnitId, int> unit_max_range;             ///< Stores the maximum attack range of each unit type.
        
        // Maps to store weapon names
        std::unordered_map<UnitId, std::string> unit_primaryweapon; ///< Stores the primary weapon name for each unit.
        std::unordered_map<UnitId, std::string> unit_secondaryweapon; ///< Stores the secondary weapon name for each unit.
        
        // Maps to store weapon damage tables
        std::unordered_map<UnitId, std::unordered_map<UnitId, int>> primaryweapon_damage; ///< Stores damage values for primary weapons against different unit types.
        std::unordered_map<UnitId, std::unordered_map<UnitId, int>> secondaryweapon_damage; ///< Stores damage values for secondary weapons against different unit types.
    };

} // namespace advanced_wars

