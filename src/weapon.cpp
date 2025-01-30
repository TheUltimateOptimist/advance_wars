#include "weapon.hpp"

namespace advanced_wars
{
Weapon::Weapon() : name(""), damage() {}

Weapon::Weapon(const std::string& weaponName, const std::unordered_map<UnitId, int>& damageValues)
    : name(weaponName), damage(damageValues)
{
}

// Funktion zum Hinzufügen von Schadenswerten
void Weapon::addDamageValue(UnitId unitId, int value)
{
    damage[unitId] = value;
}

// Funktion zum Abrufen eines Schadenswertes
int Weapon::getDamageValue(UnitId unitId) const
{
    auto it = damage.find(unitId);
    if (it != damage.end())
    {
        return it->second;
    }
    return 0; // oder ein Fehlerwert
}

} // namespace advanced_wars