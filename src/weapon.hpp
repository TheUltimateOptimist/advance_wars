#pragma once

#include <string>
#include <unordered_map>

namespace advanced_wars
{

enum class UnitId;

class Weapon
{
    public:
        // Konstruktoren
        Weapon();
        Weapon(const std::string& weaponName, const std::unordered_map<UnitId, int>& damageValues);

        // Methode, um einen Schadenswert hinzuzufügen
        void addDamageValue(UnitId unitId, int value);

        // Methode, um einen Schadenswert abzurufen
        int getDamageValue(UnitId unitId) const;

        // Name der Waffe
        std::string name;

        // Schadenstabelle
        std::unordered_map<UnitId, int> damage;
};

} // namespace advanced_wars
