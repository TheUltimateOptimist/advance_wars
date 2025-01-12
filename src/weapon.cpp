#include <tinyxml2.h>
#include "weapon.hpp"
#include <iostream>
#include <vector>

using namespace tinyxml2;

std::vector<Weapon> loadWeaponsFromXML(const std::string& fileName) {
    std::vector<Weapon> weapons;
    XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Failed to load XML file!" << std::endl;
        return weapons;
    }

    XMLElement* root = doc.FirstChildElement("Weapons");
    for (XMLElement* weaponElement = root->FirstChildElement("Weapon"); weaponElement != nullptr; weaponElement = weaponElement->NextSiblingElement("Weapon")) {
        std::string name = weaponElement->FirstChildElement("Name")->GetText();
        std::string typeStr = weaponElement->FirstChildElement("Type")->GetText();
        int damage = std::stoi(weaponElement->FirstChildElement("BaseDamage")->GetText());
        int ammo = std::stoi(weaponElement->FirstChildElement("Ammunition")->GetText());

        // Map string to WeaponType enum
        WeaponType type;
        if (typeStr == "SMALL_ARMS") type = WeaponType::SMALL_ARMS;
        else if (typeStr == "ANTI_TANK") type = WeaponType::ANTI_TANK;
        else if (typeStr == "ARTILLERY") type = WeaponType::ARTILLERY;
        else if (typeStr == "ANTI_AIR") type = WeaponType::ANTI_AIR;
        else if (typeStr == "MISSILES") type = WeaponType::MISSILES;

        // Parse Traits
        XMLElement* traitsElement = weaponElement->FirstChildElement("Traits");
        bool canHitGround = std::string(traitsElement->FirstChildElement("CanHitGround")->GetText()) == "true";
        bool canHitAir = std::string(traitsElement->FirstChildElement("CanHitAir")->GetText()) == "true";
        bool canHitNaval = std::string(traitsElement->FirstChildElement("CanHitNaval")->GetText()) == "true";
        int rangeMin = std::stoi(traitsElement->FirstChildElement("RangeMin")->GetText());
        int rangeMax = std::stoi(traitsElement->FirstChildElement("RangeMax")->GetText());

        WeaponTraits traits(canHitGround, canHitAir, canHitNaval, rangeMin, rangeMax);
        weapons.emplace_back(name, type, damage, ammo, traits);
    }

    return weapons;
}