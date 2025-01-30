#include "unit.hpp"
#include <iostream>

namespace advanced_wars
{

    Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state)
        : x(x), y(y), faction(faction), id(id), state(state), max_health(100)
    {
        health = max_health;
    };

    void Unit::render(Engine &engine, int scale)
    {
        Spritesheet *spritesheet = engine.get_spritesheet();

        int step = engine.get_stage() % spritesheet->get_unit_textures()
                                            .at(static_cast<int>(faction))
                                            .at(static_cast<int>(id))
                                            .at(static_cast<int>(state))
                                            .second;

        if (state == UnitState::IDLE || state == UnitState::UNAVAILABLE)
        {

            SDL_Rect src;
            src.x = step * spritesheet->get_unit_width();
            src.y = 0;
            src.w = spritesheet->get_unit_width();
            src.h = spritesheet->get_unit_height();

            SDL_Rect dst;
            dst.x = x * spritesheet->get_unit_width() * scale;
            dst.y = y * spritesheet->get_unit_height() * scale;
            dst.w = spritesheet->get_unit_width() * scale;
            dst.h = spritesheet->get_unit_height() * scale;

            SDL_RenderCopyEx(engine.renderer(),
                             spritesheet->get_unit_textures()
                                 .at(static_cast<int>(faction))
                                 .at(static_cast<int>(id))
                                 .at(static_cast<int>(state))
                                 .first,
                             &src, &dst, 0, NULL, SDL_FLIP_NONE);
        }
        else
        {
            // The moving states have a resolution of 24x24 instead of 16x16 and need to
            // be handled separately
            SDL_Rect src;
            src.x = step * spritesheet->get_unit_moving_width();
            src.y = 0;
            src.w = spritesheet->get_unit_moving_width();
            src.h = spritesheet->get_unit_moving_height();

            SDL_Rect dst;
            dst.x = ((x * spritesheet->get_unit_width()) - 4) * scale;
            dst.y = ((y * spritesheet->get_unit_height()) - 4) * scale;
            dst.w = spritesheet->get_unit_moving_width() * scale;
            dst.h = spritesheet->get_unit_moving_height() * scale;

            SDL_RenderCopyEx(engine.renderer(),
                             spritesheet->get_unit_textures()
                                 .at(static_cast<int>(faction))
                                 .at(static_cast<int>(id))
                                 .at(static_cast<int>(state))
                                 .first,
                             &src, &dst, 0, NULL, SDL_FLIP_NONE);
        }
    }
    
    void Unit::attack(Unit *enemy)
    {   
        secondary_weapon = fill_matchupTable(0);
        primary_weapon = fill_matchupTable(1);

        

        // Zuerst die Tabel für die Waffen der angreifenden Einheit holen
        auto &attackerSecondaryWeaponTable = secondary_weapon[this->id];
        auto &attackerPrimaryWeaponTable = primary_weapon[this->id];

        

        // Schadenswert für die angreifende Einheit gegen die verteidigende Einheit berechnen
        // Es wird die Waffe genommen die mehr Schaden macht

        int attackerDamageValue = 0;

        if (attackerSecondaryWeaponTable.find(enemy->id) != attackerSecondaryWeaponTable.end())
        {
            attackerDamageValue = attackerSecondaryWeaponTable[enemy->id];
        }
        
        if (attackerPrimaryWeaponTable.find(enemy->id) != attackerPrimaryWeaponTable.end())
        {
            if (attackerDamageValue < attackerPrimaryWeaponTable[enemy->id])
            {
                // Here ammo deduction should happen if applicable
                attackerDamageValue = attackerPrimaryWeaponTable[enemy->id];
            }
        }

        

        if (attackerDamageValue == 0)
        {
            std::cout << "No damage value found for attack from unit " << static_cast<int>(id)
                      << " against unit " << static_cast<int>(enemy->id) << std::endl;
        }
        else
        {

            int offDamage = attackerDamageValue * (static_cast<float>(health) / max_health);
            enemy->health -= offDamage;
            enemy->health = std::max(0, enemy->health); // Ensuring health is not negative
            std::cout << "Enemy health after attack: " << enemy->health << std::endl;

            // Prüfen, ob der Gegner noch am Leben ist um zurückzuschlagen
            if (enemy->health > 0)
            {
                // Weapon tables for the defender
                auto &defenderSecondaryWeaponTable = secondary_weapon[enemy->id];
                auto &defenderPrimaryWeaponTable = primary_weapon[enemy->id];

                int defenderDamageValue = 0; // Declare outside for later use

                // Determine the damage value for the defender
                if (defenderSecondaryWeaponTable.find(id) != defenderSecondaryWeaponTable.end())
                {
                    defenderDamageValue = defenderSecondaryWeaponTable[id];
                }
                if (defenderPrimaryWeaponTable.find(id) != defenderPrimaryWeaponTable.end())
                {
                    if (defenderDamageValue < defenderPrimaryWeaponTable[id])
                    {
                        // Deduct ammo for primary weapon, if applicable
                        defenderDamageValue = defenderPrimaryWeaponTable[id];
                    }
                }

                // If a valid damage value was determined for retaliation
                if (defenderDamageValue > 0)
                {
                    int defDamage = static_cast<int>(defenderDamageValue * static_cast<float>(enemy->health) / enemy->max_health);
                    this->health -= defDamage;
                    this->health = std::max(0, this->health); // Safeguard against negative health
                    std::cout << "Ally health after retaliation: " << this->health << std::endl;
                }
            }
        }
    }

    MatchupTable Unit::fill_matchupTable(int type) {
        switch (type)
        {
        case 0:
            
            break;
        
        default:
            break;
        }
    }

    void Unit::update_position(int posX, int posY)
    {
        this->x = posX;
        this->y = posY;       
    }      
    

/*
Features:
//select unit 
    - show context menu
    - show move range
    - MAYBE show valid targets

//deselect unit

//attack unit
    - show context menu

*/
    void Unit::onClick(SDL_Event event, std::vector<Unit> &unitVector)
    {

        Unit *defender = nullptr;
        Unit *attacker = nullptr;

        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:

            // we have to re-initialize the unit.state (probably to idle)
            this->is_selected = true;
            std::cout << "I am selected!!" << std::endl;
            std::cout << "And my position is:" << this->x << " " << this->y << std::endl;

           //make move range calc

            break;
        case SDL_BUTTON_RIGHT:

            this->is_targeted = true;
            std::cout << "I am targeted!!" << std::endl;
            std::cout << "And my position is:" << this->x << " " << this->y << std::endl;

            for (Unit unit : unitVector)
            {
                if (unit.state == advanced_wars::UnitState::UNAVAILABLE)
                {
                    continue;
                }

                if (unit.is_selected)
                {
                    attacker = &unit;
                }

                if (unit.is_targeted)
                {
                    defender = &unit;
                }
            }

            if (attacker != nullptr && defender != nullptr)
            {
                //attack(attacker, defender);
                std::cout << "We are fighting!!" << std::endl;
                break;
            }
            else
            {
                std::cerr << "Angriff konnte nicht gestartet werden!" << std::endl;
                break;
            }
        }
    }

    bool Unit::inRange(Unit *enemy)
    {
        if (this->x == enemy->x)
        {
            return abs(this->y - enemy->y) <= this->range;
        }
        else if (this->y == enemy->y)
        {
            return abs(this->x - enemy->x) <= this->range;
        }
        return false;
    }

} // namespace advanced_wars