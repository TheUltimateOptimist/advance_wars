#ifndef WEAPON_HPP
#define WEAPON_HPP
#include <string>

enum class WeaponType {
    SMALL_ARMS = 0,
    ANTI_TANK,
    ARTILLERY,
    ANTI_AIR,
    MISSILES
};

struct WeaponTraits {
    bool can_hit_ground = false;
    bool can_hit_air = false;
    bool can_hit_naval = false;
    int range_min = 1;
    int range_max = 1;

    WeaponTraits(bool ground, bool air, bool naval, int min_range, int max_range) 
        : can_hit_ground(ground), can_hit_air(air), can_hit_naval(naval),
          range_min(min_range), range_max(max_range) {}
};


class Weapon {

    public:
    Weapon(std::string name, WeaponType type, int damage, int ammo, WeaponTraits caps)
    : name(name), type(type), base_damage(damage), ammunition(ammo), traits(caps) {}

    private:
    std::string name;
    WeaponType type;
    int base_damage;
    int ammunition;
    WeaponTraits traits;

};

#endif