#include "Weapon.hpp"

namespace advanced_wars
{
Weapon::Weapon() : m_name(""), m_damage() {}

Weapon::Weapon(const std::string& weaponName, const std::unordered_map<UnitId, int>& damageValues)
    : m_name(weaponName), m_damage(damageValues)
{
}

// Funktion zum Hinzufügen von Schadenswerten
void Weapon::addDamageValue(UnitId unitId, int value)
{
    m_damage[unitId] = value;
}

// Funktion zum Abrufen eines Schadenswertes
int Weapon::getDamageValue(UnitId unitId) const
{
    auto it = m_damage.find(unitId);
    if (it != m_damage.end())
    {
        return it->second;
    }
    return 0; // oder ein Fehlerwert
}

std::unordered_map<UnitId, int>& Weapon::getDamage()
{
    return m_damage;
}

} // namespace advanced_wars