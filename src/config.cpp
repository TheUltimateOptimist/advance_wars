#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>
#include "unit.hpp"
#include "config.hpp"

namespace advanced_wars
{

    Config::Config(const std::string &filename)
    {
        namespace pt = boost::property_tree;
        pt::ptree tree;
        pt::read_xml(filename, tree);

        for (const auto &unit : tree.get_child("Units"))
        {
            if (unit.first != "Unit")
                continue;
            const auto &unitData = unit.second;

            std::string unit_key = unitData.get<std::string>("<xmlattr>.key");
            UnitId unitId = map_unit_key_to_id(unit_key);
            if (unitId == UnitId::UNKNOWN)
            {
                continue;
            }

            unit_costs[unitId] = unitData.get<int>("Cost");
            unit_movement_points[unitId] = unitData.get<int>("MovementPoints");
            unit_ammo[unitId] = unitData.get<int>("Ammo");
            unit_min_range[unitId] = unitData.get<int>("minRange", 0);
            unit_max_range[unitId] = unitData.get<int>("maxRange", 0);
            unit_movement_type[unitId] = map_movement_type(unitData.get<std::string>("MovementType"));

            for (const auto &weapon : unitData.get_child("Weapons"))
            {
                if (weapon.first == "PrimaryWeapon")
                {
                    unit_primary_weapon[unitId] = weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto &damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId targetId = map_unit_key_to_id(target_key);
                        if (targetId == UnitId::UNKNOWN)
                            continue;

                        primary_weapon_damage[unitId][targetId] = damage.second.get<int>("<xmlattr>.value");
                    }
                }
                else if (weapon.first == "SecondaryWeapon")
                {
                    unit_secondary_weapon[unitId] = weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto &damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId targetId = map_unit_key_to_id(target_key);
                        if (targetId == UnitId::UNKNOWN)
                            continue;

                        secondary_weapon_damage[unitId][targetId] = damage.second.get<int>("<xmlattr>.value");
                    }
                }
            }
        }
    }

    UnitId Config::map_unit_key_to_id(const std::string &unit_key) const
    {
        static const std::unordered_map<std::string, UnitId> unit_map = {
            {"infantry", UnitId::INFANTERY},
            {"mechanized_infantry", UnitId::MECHANIZED_INFANTERY},
            {"recon", UnitId::RECON},
            {"apc", UnitId::APC},
            {"anti_air", UnitId::ANTI_AIR},
            {"tank", UnitId::TANK},
            {"md_tank", UnitId::MD_TANK},
            {"neotank", UnitId::NEO_TANK},
            {"artillery", UnitId::ARTILLERY},
            {"rocket", UnitId::ROCKET_ARTILLERY},
            {"missile", UnitId::MISSILE},
            {"lander", UnitId::LANDER},
            {"cruiser", UnitId::CRUISER},
            {"submarine", UnitId::SUBMARINE},
            {"battleship", UnitId::BATTLESHIP},
            {"t_copter", UnitId::TRANSPORT_HELICOPTER},
            {"b_copter", UnitId::BATTLE_HELICOPTER},
            {"fighter", UnitId::FIGHTER},
            {"bomber", UnitId::BOMBER}};

        auto it = unit_map.find(unit_key);
        return it != unit_map.end() ? it->second : UnitId::UNKNOWN;
    }

    MovementType Config::map_movement_type(const std::string &movementTypeStr) const
    {
        static const std::unordered_map<std::string, MovementType> movement_map = {
            {"Foot", MovementType::FOOT},
            {"Wheeled", MovementType::WHEELED},
            {"Tread", MovementType::TREAD},
            {"Air", MovementType::AIR},
            {"Sea", MovementType::SEA},
            {"Lander", MovementType::LANDER}

        };
        auto it = movement_map.find(movementTypeStr);
        return it != movement_map.end() ? it->second : MovementType::UNKNOWN;
    }

    int Config::get_unit_cost(UnitId id) const
    {
        auto it = unit_costs.find(id);
        if (it != unit_costs.end())
        {
            return it->second;
        }
        throw std::runtime_error("Cost for unit ID not found");
    }

    int Config::get_unit_movement_points(UnitId id) const
    {
        auto it = unit_movement_points.find(id);
        if (it != unit_movement_points.end())
        {
            return it->second;
        }
        throw std::runtime_error("Movement points for unit ID not found");
    }

    MovementType Config::get_unit_movement_type(UnitId id) const
    {
        auto it = unit_movement_type.find(id);
        if (it != unit_movement_type.end())
        {
            return it->second;
        }
        throw std::runtime_error("Movement type for unit ID not found");
    }

    int Config::get_unit_ammo(UnitId id) const
    {
        auto it = unit_ammo.find(id);
        if (it != unit_ammo.end())
        {
            return it->second;
        }
        throw std::runtime_error("Ammo for unit ID not found");
    }

    int Config::get_unit_min_range(UnitId id) const
    {
        auto it = unit_min_range.find(id);
        if (it != unit_min_range.end())
        {
            return it->second;
        }
        throw std::runtime_error("Min range for unit ID not found");
    }

    int Config::get_unit_max_range(UnitId id) const
    {
        auto it = unit_max_range.find(id);
        if (it != unit_max_range.end())
        {
            return it->second;
        }
        throw std::runtime_error("Max range for unit ID not found");
    }

    std::string Config::get_unit_primary_weapon(UnitId id) const
    {
        auto it = unit_primary_weapon.find(id);
        if (it != unit_primary_weapon.end())
        {
            return it->second;
        }
        throw std::runtime_error("Primary weapon for unit ID not found");
    }

    std::string Config::get_unit_secondary_weapon(UnitId id) const
    {
        auto it = unit_secondary_weapon.find(id);
        if (it != unit_secondary_weapon.end())
        {
            return it->second;
        }
        throw std::runtime_error("Secondary weapon for unit ID not found");
    }

    int Config::get_unit_primary_weapon_damage(UnitId attackerid, UnitId defenderid) const
    {
        auto it = primary_weapon_damage.find(attackerid);
        if (it != primary_weapon_damage.end())
        {
            auto damageIt = it->second.find(defenderid);
            if (damageIt != it->second.end())
            {
                return damageIt->second;
            }
        }
        throw std::runtime_error("Primary weapon damage not found for given attacker/defender combination");
    }

    int Config::get_unit_secondary_weapon_damage(UnitId attackerid, UnitId defenderid) const
    {
        auto it = secondary_weapon_damage.find(attackerid);
        if (it != secondary_weapon_damage.end())
        {
            auto damageIt = it->second.find(defenderid);
            if (damageIt != it->second.end())
            {
                return damageIt->second;
            }
        }
        throw std::runtime_error("Secondary weapon damage not found for given attacker/defender combination");
    }
}