#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "unit.hpp"  // Include für UnitId

namespace advanced_wars {

class Config {
   public:
       Config(MatchupTabel_secondaryweapon& secWeapon, MatchupTabel_primaryweapon& primWeapon);
       void loadFromXML(const char* filename);
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
       MatchupTabel_secondaryweapon& secondary_weapon_damage;
       MatchupTabel_primaryweapon& primary_weapon_damage;
   };

} // namespace advanced_wars

#endif // CONFIG_HPP