#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept> // Für die Ausnahmebehandlung
#include "unit.hpp"  // Include für UnitId

namespace pt = boost::property_tree;

namespace advanced_wars
{

    class Config
    {
    public:
        void loadFromXML(const char *filename);

        int get_unit_cost(UnitId id) const;
        int get_unit_movementPoints(UnitId id) const;
        int get_unit_ammo(UnitId id) const;
        int get_unit_minRange(UnitId id) const;
        int get_unit_maxRange(UnitId id) const;
        std::string get_unit_secondaryweapon(UnitId id) const;
        std::string get_unit_primaryweapon(UnitId id) const;

    private:
        std::unordered_map<UnitId, int> unit_costs;
        std::unordered_map<UnitId, int> unit_movementPoints;
        std::unordered_map<UnitId, int> unit_ammo;
        std::unordered_map<UnitId, int> unit_minRange;
        std::unordered_map<UnitId, int> unit_maxRange;
        std::unordered_map<UnitId, std::string> unit_secondaryweapon;
        std::unordered_map<UnitId, std::string> unit_primaryweapon;
    };

    void Config::loadFromXML(const char *filename)
    {
        pt::ptree tree;
        pt::read_xml(filename, tree);

        for (const auto &unit : tree.get_child("Units"))
        {
            if (unit.first == "Unit")
            {
                std::string unitKey = unit.second.get<std::string>("<xmlattr>.key");

                UnitId unitId;
                if (unitKey == "infantry")
                {
                    unitId = UnitId::INFANTERY;
                }
                else if (unitKey == "Mech")
                {
                    unitId = UnitId::MECH;
                }
                else
                {
                    continue; // Überspringt nicht unterstützte Einheiten
                }

                int cost = unit.second.get<int>("Cost");
                int movementPoints = unit.second.get<int>("MovementPoints");
                int ammo = unit.second.get<int>("Ammo");
                int minRange = unit.second.get<int>("minRange", 0);
                int maxRange = unit.second.get<int>("maxRange", 0);

                unit_costs[unitId] = cost;
                unit_movementPoints[unitId] = movementPoints;
                unit_ammo[unitId] = ammo;
                unit_minRange[unitId] = minRange;
                unit_maxRange[unitId] = maxRange;

                for (const auto &weapon : unit.second.get_child("Weapons"))
                {
                    if (weapon.first == "PrimaryWeapon")
                    {
                        std::string weaponName = weapon.second.get<std::string>("<xmlattr>.name");
                        unit_primaryweapon[unitId] = weaponName;
                    }
                    else if (weapon.first == "SecondaryWeapon")
                    {
                        std::string weaponName = weapon.second.get<std::string>("<xmlattr>.name");
                        unit_secondaryweapon[unitId] = weaponName;
                    }
                }
            }
        }
    }

    // Definitions for the accessor methods
    int Config::get_unit_cost(UnitId id) const
    {
        auto it = unit_costs.find(id);
        if (it != unit_costs.end())
            return it->second;
        throw std::runtime_error("Cost for unit ID not found");
    }

    int Config::get_unit_movementPoints(UnitId id) const
    {
        auto it = unit_movementPoints.find(id);
        if (it != unit_movementPoints.end())
            return it->second;
        throw std::runtime_error("Movement points for unit ID not found");
    }

    int Config::get_unit_ammo(UnitId id) const
    {
        auto it = unit_ammo.find(id);
        if (it != unit_ammo.end())
            return it->second;
        throw std::runtime_error("Ammo for unit ID not found");
    }

    int Config::get_unit_minRange(UnitId id) const
    {
        auto it = unit_minRange.find(id);
        if (it != unit_minRange.end())
            return it->second;
        throw std::runtime_error("Min range for unit ID not found");
    }

    int Config::get_unit_maxRange(UnitId id) const
    {
        auto it = unit_maxRange.find(id);
        if (it != unit_maxRange.end())
            return it->second;
        throw std::runtime_error("Max range for unit ID not found");
    }

    std::string Config::get_unit_secondaryweapon(UnitId id) const
    {
        auto it = unit_secondaryweapon.find(id);
        if (it != unit_secondaryweapon.end())
            return it->second;
        throw std::runtime_error("Secondary weapon for unit ID not found");
    }

    std::string Config::get_unit_primaryweapon(UnitId id) const
    {
        auto it = unit_primaryweapon.find(id);
        if (it != unit_primaryweapon.end())
            return it->second;
        throw std::runtime_error("Primary weapon for unit ID not found");
    }

} // namespace advanced_wars
