#include "Config.hpp"
#include "Unit.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace advanced_wars
{

Config::Config(const std::string& filename)
{
    namespace pt = boost::property_tree;
    pt::ptree tree;
    pt::read_xml(filename, tree);

    for (const auto& unit : tree.get_child("Units"))
    {
        if (unit.first != "Unit")
            continue;
        const auto& unitData = unit.second;

        std::string unit_key = unitData.get<std::string>("<xmlattr>.key");
        try
        {
            UnitId unitId = mapUnitKeyToID(unit_key);

            m_unitCosts[unitId] = unitData.get<int>("Cost");
            m_unitMovementPoints[unitId] = unitData.get<int>("MovementPoints");
            m_unitAmmo[unitId] = unitData.get<int>("Ammo");
            m_unitMinRange[unitId] = unitData.get<int>("minRange", 0);
            m_unitMaxRange[unitId] = unitData.get<int>("maxRange", 0);

            std::string movement_type_str = unitData.get<std::string>("MovementType");
            try
            {
                m_unitMovementType[unitId] = mapMovementType(movement_type_str);
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "Unknown movement type: " << movement_type_str
                          << " for unit key: " << unit_key << std::endl;
                continue;
            }

            for (const auto& weapon : unitData.get_child("Weapons"))
            {
                if (weapon.first == "PrimaryWeapon")
                {
                    m_unitPrimaryWeapon[unitId] = weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto& damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId      targetId = mapUnitKeyToID(target_key);
                        m_primaryWeaponDamage[unitId][targetId] =
                            damage.second.get<int>("<xmlattr>.value");
                    }
                }
                else if (weapon.first == "SecondaryWeapon")
                {
                    m_unitSecondaryWeapon[unitId] =
                        weapon.second.get<std::string>("<xmlattr>.name");

                    for (const auto& damage : weapon.second.get_child("DamageTable"))
                    {
                        if (damage.first != "Damage")
                            continue;

                        std::string target_key = damage.second.get<std::string>("<xmlattr>.unitId");
                        UnitId      targetId = mapUnitKeyToID(target_key);
                        m_secondaryWeaponDamage[unitId][targetId] =
                            damage.second.get<int>("<xmlattr>.value");
                    }
                }
            }
        }
        catch (const std::out_of_range& e)
        {
            // std::cerr << "Unknown unit key: " << unit_key << std::endl;
            continue;
        }
    }
}

UnitId Config::mapUnitKeyToID(const std::string& unit_key) const
{
    static const std::unordered_map<std::string, UnitId> unit_map = {
        {                 "infantry",                 UnitId::INFANTERY},
        {      "mechanized_infantry",      UnitId::MECHANIZED_INFANTERY},
        {                    "recon",                     UnitId::RECON},
        {                      "apc",                       UnitId::APC},
        {            "anti_air_tank",             UnitId::ANTI_AIR_TANK},
        {              "medium_tank",               UnitId::MEDIUM_TANK},
        {               "heavy_tank",                UnitId::HEAVY_TANK},
        {                  "neotank",                  UnitId::NEO_TANK},
        {                "artillery",                 UnitId::ARTILLERY},
        {         "rocket_artillery",          UnitId::ROCKET_ARTILLERY},
        {"anti_air_missile_launcher", UnitId::ANTI_AIR_MISSILE_LAUNCHER},
        {                   "lander",                    UnitId::LANDER},
        {                  "cruiser",                   UnitId::CRUISER},
        {                "submarine",                 UnitId::SUBMARINE},
        {               "battleship",                UnitId::BATTLESHIP},
        {     "transport_helicopter",      UnitId::TRANSPORT_HELICOPTER},
        {        "battle_helicopter",         UnitId::BATTLE_HELICOPTER},
        {                  "fighter",                   UnitId::FIGHTER},
        {                   "bomber",                    UnitId::BOMBER}
    };

    auto it = unit_map.find(unit_key);
    if (it != unit_map.end())
    {
        return it->second;
    }
    throw std::out_of_range("Unknown unit key: " + unit_key);
}

MovementType Config::mapMovementType(const std::string& movementTypeStr) const
{
    static const std::unordered_map<std::string, MovementType> movement_map = {
        {   "Foot",    MovementType::FOOT},
        {"Wheeled", MovementType::WHEELED},
        {  "Tread",   MovementType::TREAD},
        {    "Air",     MovementType::AIR},
        {    "Sea",     MovementType::SEA},
        { "Lander",  MovementType::LANDER}
    };

    auto it = movement_map.find(movementTypeStr);
    if (it != movement_map.end())
    {
        return it->second;
    }
    throw std::out_of_range("Unknown movement type: " + movementTypeStr);
}

int Config::getUnitCost(UnitId id) const
{
    auto it = m_unitCosts.find(id);
    if (it != m_unitCosts.end())
    {
        return it->second;
    }
    throw std::runtime_error("Cost for unit ID not found");
}

int Config::getUnitMovementPoints(UnitId id) const
{
    auto it = m_unitMovementPoints.find(id);
    if (it != m_unitMovementPoints.end())
    {
        return it->second;
    }
    throw std::runtime_error("Movement points for unit ID not found");
}

MovementType Config::getUnitMovementType(UnitId id) const
{
    auto it = m_unitMovementType.find(id);
    if (it != m_unitMovementType.end())
    {
        return it->second;
    }
    throw std::runtime_error("Movement type for unit ID not found");
}

int Config::getUnitAmmo(UnitId id) const
{
    auto it = m_unitAmmo.find(id);
    if (it != m_unitAmmo.end())
    {
        return it->second;
    }
    throw std::runtime_error("Ammo for unit ID not found");
}

int Config::getUnitMinRange(UnitId id) const
{
    auto it = m_unitMinRange.find(id);
    if (it != m_unitMinRange.end())
    {
        return it->second;
    }
    throw std::runtime_error("Min range for unit ID not found");
}

int Config::getUnitMaxRange(UnitId id) const
{
    auto it = m_unitMaxRange.find(id);
    if (it != m_unitMaxRange.end())
    {
        return it->second;
    }
    throw std::runtime_error("Max range for unit ID not found");
}

std::string Config::getUnitPrimaryWeapon(UnitId id) const
{
    auto it = m_unitPrimaryWeapon.find(id);
    if (it != m_unitPrimaryWeapon.end())
    {
        return it->second;
    }
    return "";
}

std::string Config::getUnitSecondaryWeapon(UnitId id) const
{
    auto it = m_unitSecondaryWeapon.find(id);
    if (it != m_unitSecondaryWeapon.end())
    {
        return it->second;
    }
    return "";
}

std::optional<int> Config::getUnitPrimaryWeaponDamage(UnitId attackerId, UnitId targetId) const {
        auto attackerMapIt = m_primaryWeaponDamage.find(attackerId);
        if (attackerMapIt != m_primaryWeaponDamage.end()) {
            auto damageIt = attackerMapIt->second.find(targetId);
            if (damageIt != attackerMapIt->second.end()) {
                return damageIt->second;
            }
        }
        // Kein spezifischer Schaden vorhanden
        return std::nullopt;
    }

std::optional<int> Config::getUnitSecondaryWeaponDamage(UnitId attackerId, UnitId targetId) const {
        auto attackerMapIt = m_secondaryWeaponDamage.find(attackerId);
        if (attackerMapIt != m_secondaryWeaponDamage.end()) {
            auto damageIt = attackerMapIt->second.find(targetId);
            if (damageIt != attackerMapIt->second.end()) {
                return damageIt->second;
            }
        }
        // Kein spezifischer Schaden vorhanden
        return std::nullopt;
    }
} // namespace advanced_wars