#include "unit.hpp"
#include <iostream>

namespace advanced_wars
{

    Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state)
        : x(x), y(y), faction(faction), id(id), state(state), max_health(100)
    {
        //das ist nur für Testzwecke
        if (id == UnitId::INFANTERY)
        {
            secondary_weapon = Weapon("Machine-Gun", {{UnitId::INFANTERY, 55}});
        }
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
        // Angenommen, primary_weapon und secondary_weapon wurden bereits korrekt initialisiert
        auto primary_weapon_damage_it = primary_weapon.damage.find(enemy->id);
        auto secondary_weapon_damage_it = secondary_weapon.damage.find(enemy->id);

        int attacker_damage_value = 0;

        // Die Waffe mit dem höchsten Schaden wählen
        if (secondary_weapon_damage_it != secondary_weapon.damage.end())
        {
            attacker_damage_value = secondary_weapon_damage_it->second;
        }

        if (primary_weapon_damage_it != primary_weapon.damage.end())
        {
            if (primary_weapon_damage_it->second > attacker_damage_value)
            {
                // Munitionsabzug sollte hier erfolgen, falls zutreffend
                attacker_damage_value = primary_weapon_damage_it->second;
            }
        }

        if (attacker_damage_value == 0)
        {
            std::cout << "No damage value found for attack from unit " << static_cast<int>(id)
                      << " against unit " << static_cast<int>(enemy->id) << std::endl;
        }
        else
        {
            int off_damage = attacker_damage_value * (static_cast<float>(health) / max_health);
            enemy->health -= off_damage;
            enemy->health = std::max(0, enemy->health); // Sicherstellen, dass die Gesundheit nicht negativ wird
            std::cout << "Enemy health after attack: " << enemy->health << std::endl;

            // Prüfen, ob der Gegner noch am Leben ist um zurückzuschlagen
            if (enemy->health > 0)
            {
                // Weapon tables for the defender
                auto defender_primary_weapon_damage_it = enemy->primary_weapon.damage.find(id);
                auto defender_secondary_weapon_damage_it = enemy->secondary_weapon.damage.find(id);

                int defender_damage_value = 0; // Declare outside for later use

                // Determine the damage value for the defender
                if (defender_secondary_weapon_damage_it != enemy->secondary_weapon.damage.end())
                {
                    defender_damage_value = defender_secondary_weapon_damage_it->second;
                }

                if (defender_primary_weapon_damage_it != enemy->primary_weapon.damage.end())
                {
                    if (defender_primary_weapon_damage_it->second > defender_damage_value)
                    {
                        // Munitionsabzug für primäre Waffe, falls zutreffend
                        defender_damage_value = defender_primary_weapon_damage_it->second;
                    }
                }

                // If a valid damage value was determined for retaliation
                if (defender_damage_value > 0)
                {
                    int def_damage = static_cast<int>(defender_damage_value * static_cast<float>(enemy->health) / enemy->max_health);
                    this->health -= def_damage;
                    this->health = std::max(0, this->health); // Safeguard against negative health
                    std::cout << "Ally health after retaliation: " << this->health << std::endl;
                }
            }
        }
    }

    MatchupTable Unit::fill_matchupTable(int type)
    {
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
        calc_state(posX, posY);

        this->x = posX;
        this->y = posY;
    }

    void Unit::calc_state(int posX, int posY) {

        int currentX = this->x;
        int currentY = this->y;

        int deltaX = currentX - posX;
        int deltaY = currentY - posY;

        if (deltaY == 0) {
            if (deltaX > 0) {
                this->state = advanced_wars::UnitState::MOVEMENTLEFT;
                return;
            } else {
                this->state = advanced_wars::UnitState::MOVEMENTRIGHT;
                return;
            }
        }

        double bresen = deltaX/deltaY;

        if(bresen == 0) {
            if(deltaY < 0) {
                this->state = advanced_wars::UnitState::MOVEMENTDOWN;
                return;
            } else {
                this->state = advanced_wars::UnitState::MOVEMENTUP;
                return;
            }
        }

        if(0 < bresen && bresen < 1) {
            this->state = advanced_wars::UnitState::MOVEMENTDOWN;
            return;
        } else if (-1 < bresen && bresen < 0) {
            this->state = advanced_wars::UnitState::MOVEMENTUP;
            return;
        }

    }

    void Unit::on_left_click(SDL_Event event, std::vector<Unit> &unitVector)
    {

        std::cout << "Left-button pressed on unit: " << this->health << std::endl;
        
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