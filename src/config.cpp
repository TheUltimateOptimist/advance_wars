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

            UnitId unitId;
            if (unit_key == "infantry")
            {
                unitId = UnitId::INFANTERY;
            }
            else if (unit_key == "Mech")
            {
                unitId = UnitId::MECHANIZED_INFANTERY;
            }
            else
            {
                continue; // Skip unsupported units
            }

            // Load basic unit properties
            unit_costs[unitId] = unitData.get<int>("Cost");
            unit_movement_points[unitId] = unitData.get<int>("MovementPoints");
            unit_ammo[unitId] = unitData.get<int>("Ammo");
            unit_min_range[unitId] = unitData.get<int>("minRange", 0);
            unit_max_range[unitId] = unitData.get<int>("maxRange", 0);

            // Process weapons
            for (const auto &weapon : unitData.get_child("Weapons"))
            {
                if (weapon.first == "PrimaryWeapon")
                {
                    unit_primaryweapon[unitId] = weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto &damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId targetId;

                        if (target_key == "infantry")
                        {
                            targetId = UnitId::INFANTERY;
                        }
                        else if (target_key == "Mech")
                        {
                            targetId = UnitId::MECHANIZED_INFANTERY;
                        }
                        else
                        {
                            continue;
                        }

                        primaryweapon_damage[unitId][targetId] = damage.second.get<int>("<xmlattr>.value");
                    }
                }
                else if (weapon.first == "SecondaryWeapon")
                {
                    unit_secondaryweapon[unitId] = weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto &damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId targetId;

                        if (target_key == "infantry")
                        {
                            targetId = UnitId::INFANTERY;
                        }
                        else if (target_key == "Mech")
                        {
                            targetId = UnitId::MECHANIZED_INFANTERY;
                        }
                        else
                        {
                            continue;
                        }

                        secondaryweapon_damage[unitId][targetId] =
                            damage.second.get<int>("<xmlattr>.value");
                    }
                }
            }
        }
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

    std::string Config::get_unit_primaryweapon(UnitId id) const
    {
        auto it = unit_primaryweapon.find(id);
        if (it != unit_primaryweapon.end())
        {
            return it->second;
        }
        throw std::runtime_error("Primary weapon for unit ID not found");
    }

    std::string Config::get_unit_secondaryweapon(UnitId id) const
    {
        auto it = unit_secondaryweapon.find(id);
        if (it != unit_secondaryweapon.end())
        {
            return it->second;
        }
        throw std::runtime_error("Secondary weapon for unit ID not found");
    }

    int Config::get_unit_primaryweapon_damage(UnitId attackerid, UnitId defenderid) const
    {
        auto it = primaryweapon_damage.find(attackerid);
        if (it != primaryweapon_damage.end())
        {
            auto damageIt = it->second.find(defenderid);
            if (damageIt != it->second.end())
            {
                return damageIt->second;
            }
        }
        throw std::runtime_error("Primary weapon damage not found for given attacker/defender combination");
    }

    int Config::get_unit_secondaryweapon_damage(UnitId attackerid, UnitId defenderid) const
    {
        auto it = secondaryweapon_damage.find(attackerid);
        if (it != secondaryweapon_damage.end())
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