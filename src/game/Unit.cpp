#include "Unit.hpp"
#include <iostream>

namespace advanced_wars
{

Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state)
    : m_x(x), m_y(y), m_faction(faction), m_id(id), m_state(state), m_maxHealth(100)
{
    // das ist nur für Testzwecke
    if (m_id == UnitId::INFANTERY)
    {
        m_secondaryWeapon = Weapon(
            "Machine-Gun", {
                               {UnitId::INFANTERY, 55}
        });
    }
    m_health = m_maxHealth;
}

void Unit::render(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.getSpritesheet();

    int step = engine.getStage() % spritesheet->getUnitTextures()
                                       .at(static_cast<int>(m_faction))
                                       .at(static_cast<int>(m_id))
                                       .at(static_cast<int>(m_state))
                                       .second;

    if (m_state == UnitState::IDLE || m_state == UnitState::UNAVAILABLE)
    {

        SDL_Rect src;
        src.x = step * spritesheet->getUnitWidth();
        src.y = 0;
        src.w = spritesheet->getUnitWidth();
        src.h = spritesheet->getUnitHeight();

        SDL_Rect dst;
        dst.x = m_x * spritesheet->getUnitWidth() * scale;
        dst.y = m_y * spritesheet->getUnitHeight() * scale;
        dst.w = spritesheet->getUnitWidth() * scale;
        dst.h = spritesheet->getUnitHeight() * scale;

        SDL_RenderCopyEx(
            engine.renderer(),
            spritesheet->getUnitTextures()
                .at(static_cast<int>(m_faction))
                .at(static_cast<int>(m_id))
                .at(static_cast<int>(m_state))
                .first,
            &src, &dst, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        // The moving states have a resolution of 24x24 instead of 16x16 and need to
        // be handled separately
        SDL_Rect src;
        src.x = step * spritesheet->getUnitMovingWidth();
        src.y = 0;
        src.w = spritesheet->getUnitMovingWidth();
        src.h = spritesheet->getUnitMovingHeight();

        SDL_Rect dst;
        dst.x = ((m_x * spritesheet->getUnitWidth()) - 4) * scale;
        dst.y = ((m_y * spritesheet->getUnitHeight()) - 4) * scale;
        dst.w = spritesheet->getUnitMovingWidth() * scale;
        dst.h = spritesheet->getUnitMovingHeight() * scale;

        SDL_RenderCopyEx(
            engine.renderer(),
            spritesheet->getUnitTextures()
                .at(static_cast<int>(m_faction))
                .at(static_cast<int>(m_id))
                .at(static_cast<int>(m_state))
                .first,
            &src, &dst, 0, NULL, SDL_FLIP_NONE);
    }
}

void Unit::attack(Unit& enemy)
{
    // Angenommen, primary_weapon und secondary_weapon wurden bereits korrekt
    // initialisiert
    auto primary_weapon_damage_it = m_primaryWeapon.m_damage.find(enemy.m_id);
    auto secondary_weapon_damage_it = m_secondaryWeapon.m_damage.find(enemy.m_id);

    int attacker_damage_value = 0;

    // Die Waffe mit dem höchsten Schaden wählen
    if (secondary_weapon_damage_it != m_secondaryWeapon.m_damage.end())
    {
        attacker_damage_value = secondary_weapon_damage_it->second;
    }

    if (primary_weapon_damage_it != m_primaryWeapon.m_damage.end())
    {
        if (primary_weapon_damage_it->second > attacker_damage_value)
        {
            // Munitionsabzug sollte hier erfolgen, falls zutreffend
            attacker_damage_value = primary_weapon_damage_it->second;
        }
    }

    if (attacker_damage_value == 0)
    {
        std::cout << "No damage value found for attack from unit " << static_cast<int>(m_id)
                  << " against unit " << static_cast<int>(enemy.m_id) << std::endl;
    }
    else
    {
        int off_damage = attacker_damage_value * (static_cast<float>(m_health) / m_maxHealth);
        enemy.m_health -= off_damage;
        enemy.m_health = std::max(
            0,
            enemy.m_health); // Sicherstellen, dass die Gesundheit nicht negativ wird
        std::cout << "Enemy health after attack: " << enemy.m_health << std::endl;

        // Prüfen, ob der Gegner noch am Leben ist um zurückzuschlagen
        if (enemy.m_health > 0)
        {
            // Weapon tables for the defender
            auto defender_primary_weapon_damage_it = enemy.m_primaryWeapon.m_damage.find(m_id);
            auto defender_secondary_weapon_damage_it = enemy.m_secondaryWeapon.m_damage.find(m_id);

            int defender_damage_value = 0; // Declare outside for later use

            // Determine the damage value for the defender
            if (defender_secondary_weapon_damage_it != enemy.m_secondaryWeapon.m_damage.end())
            {
                defender_damage_value = defender_secondary_weapon_damage_it->second;
            }

            if (defender_primary_weapon_damage_it != enemy.m_primaryWeapon.m_damage.end())
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
                int def_damage = static_cast<int>(
                    defender_damage_value * static_cast<float>(enemy.m_health) / enemy.m_maxHealth);
                this->m_health -= def_damage;
                this->m_health = std::max(0, this->m_health); // Safeguard against negative health
                std::cout << "Ally health after retaliation: " << this->m_health << std::endl;
            }
        }
    }
}

void Unit::updatePosition(int posX, int posY)
{
    calcState(posX, posY);

    this->m_x = posX;
    this->m_y = posY;
}

void Unit::calcState(int posX, int posY)
{
    int deltaX = this->m_x - posX;
    int deltaY = this->m_y - posY;

    if (deltaX == 0 && deltaY == 0)
    {
        // Unit is already at the target position
        return;
    }

    if (abs(deltaX) >= abs(deltaY))
    {
        if (deltaX > 0)
        {
            this->m_state = advanced_wars::UnitState::MOVEMENTLEFT;
        }
        else
        {
            this->m_state = advanced_wars::UnitState::MOVEMENTRIGHT;
        }
    }
    else
    {
        if (deltaY > 0)
        {
            this->m_state = advanced_wars::UnitState::MOVEMENTUP;
        }
        else
        {
            this->m_state = advanced_wars::UnitState::MOVEMENTDOWN;
        }
    }
}

void Unit::on_left_click(SDL_Event event)
{

    std::cout << "Left-button pressed on unit: " << this->m_health << std::endl;
}

bool Unit::inRange(Unit& enemy)
{
    if (this->m_x == enemy.m_x)
    {
        return abs(this->m_y - enemy.m_y) <= this->m_range;
    }
    else if (this->m_y == enemy.m_y)
    {
        return abs(this->m_x - enemy.m_x) <= this->m_range;
    }
    return false;
}

} // namespace advanced_wars