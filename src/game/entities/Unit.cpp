#include "Unit.hpp"
#include "../core/Config.hpp"

#include <iostream>

namespace advanced_wars
{

Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state, Config& config)
    : m_x(x), m_y(y), m_faction(faction), m_id(id), m_state(state), m_maxHealth(100)
{
    // Allgemeine Einheiteneinstellungen aus Konfiguration holen
    m_cost = config.getUnitCost(id);
    m_movementPoints = config.getUnitMovementPoints(id);
    m_ammo = config.getUnitAmmo(id);
    m_minRange = config.getUnitMinRange(id);
    m_maxRange = config.getUnitMaxRange(id);
    m_health = m_maxHealth;

    m_movementType = config.getUnitMovementType(id);

    // Initialisieren der Primär- und Sekundärwaffe
    std::unordered_map<UnitId, int> primaryDamage;
    std::unordered_map<UnitId, int> secondaryDamage;

    for (int targetIt = static_cast<int>(UnitId::FIRST); targetIt <= static_cast<int>(UnitId::LAST);
         ++targetIt)
    {
        UnitId targetId = static_cast<UnitId>(targetIt);

        // Prüfen, ob ein gültiger Schadenswert vorhanden ist, und nur dann hinzufügen
        if (auto damage = config.getUnitPrimaryWeaponDamage(id, targetId))
        {
            primaryDamage[targetId] = *damage;
        }
        if (auto damage = config.getUnitSecondaryWeaponDamage(id, targetId))
        {
            secondaryDamage[targetId] = *damage;
        }
    }

    m_primaryWeapon = Weapon(config.getUnitPrimaryWeapon(id), primaryDamage);
    m_secondaryWeapon = Weapon(config.getUnitSecondaryWeapon(id), secondaryDamage);
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
        dst.y = ((m_y * spritesheet->getUnitHeight()) - 8) * scale;
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
    renderHP(engine, scale);
}

void Unit::attack(Unit& enemy)
{
    int attacker_damage_value = calculateDamage(enemy);

    if (attacker_damage_value > 0)
    {
        performAttack(enemy, attacker_damage_value);
        std::cout << "Enemy health after attack: " << enemy.m_health << std::endl;

        // Check if the enemy is still alive for counter-attack
        if (enemy.m_health > 0)
        {
            // Check if the enemy is within attack range
            int distanceX = std::abs(enemy.m_x - m_x);
            int distanceY = std::abs(enemy.m_y - m_y);
            int distance = distanceX + distanceY;

            if (distance >= enemy.m_minRange && distance <= enemy.m_maxRange)
            {
                // Now, they are reversed for the counter-attack
                int defender_damage_value = enemy.calculateDamage(*this);
                if (defender_damage_value > 0)
                {
                    enemy.performAttack(*this, defender_damage_value);
                    std::cout << "Ally health after retaliation: " << this->m_health << std::endl;
                }
            }
            else
            {
                std::cout << "Enemy out of range for counter-attack." << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No damage value found for attack from unit " << static_cast<int>(m_id)
                  << " against unit " << static_cast<int>(enemy.m_id) << std::endl;
    }
}

int Unit::calculateDamage(Unit& target)
{
    // Pointers to Weapon objects
    Weapon* primaryWeapon = &m_primaryWeapon;
    Weapon* secondaryWeapon = &m_secondaryWeapon;

    // Find the corresponding damage values
    auto primary_damage_it = primaryWeapon->getDamage().find(target.m_id);
    auto secondary_damage_it = secondaryWeapon->getDamage().find(target.m_id);

    int damage_value = 0;

    // Calculate damage using secondary weapon if available
    if (secondary_damage_it != secondaryWeapon->getDamage().end())
    {
        damage_value = secondary_damage_it->second;
    }

    // Calculate damage using primary weapon if higher and ammo is available
    if (primary_damage_it != primaryWeapon->getDamage().end())
    {
        // Check ammo correctly
        int& ammo = m_ammo;

        if (primary_damage_it->second > damage_value && ammo > 0)
        {
            ammo -= 1;
            damage_value = primary_damage_it->second;
            std::cout << " ammo = " << ammo << std::endl;
        }
    }

    return damage_value;
}

void Unit::performAttack(Unit& target, int damage)
{
    int effective_damage = damage * (static_cast<float>(m_health) / m_maxHealth);
    target.m_health -= effective_damage;
    target.m_health = std::max(0, target.m_health);
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

std::vector<Unit*> Unit::getUnitsInRangeWithDamagePotential(const std::vector<Unit*>& allUnits)
{
    std::vector<Unit*> unitsInRangeWithDamage;

    for (Unit* unit : allUnits)
    { // Iterate over all units
        // except itself
        if (unit->getFaction() == this->m_faction)
        {
            continue;
        }

        int distanceX = std::abs(unit->m_x - m_x);
        int distanceY = std::abs(unit->m_y - m_y);

        int distance = distanceX + distanceY;
        if (distance >= m_minRange && distance <= m_maxRange)
        {
            // Prüfen ob Schaden möglich ist
            auto primaryDamageIt = m_primaryWeapon.getDamage().find(unit->m_id);
            auto secondaryDamageIt = m_secondaryWeapon.getDamage().find(unit->m_id);

            bool canDealDamage = false;

            // Prüfen, ob Primärwaffe Schaden machen kann
            if (primaryDamageIt != m_primaryWeapon.getDamage().end() && m_ammo > 0)
            {
                canDealDamage = true;
            }
            // Prüfen, ob Sekundärwaffe Schaden machen kann
            if (secondaryDamageIt != m_secondaryWeapon.getDamage().end())
            {
                canDealDamage = true;
            }

            if (canDealDamage)
            {
                unitsInRangeWithDamage.push_back(unit);
            }
        }
    }

    return unitsInRangeWithDamage;
}

void Unit::renderHP(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.getSpritesheet();

    SDL_Texture* numbers = spritesheet->getNumberTexture();
    int          numberWidth = spritesheet->getNumberWidth();
    int          numberHeight = spritesheet->getNumberHeight();

    int hp = ceil((double)m_health / 10);

    SDL_Rect src;
    src.x = hp % 10 * numberWidth;
    src.y = 0;
    src.w = numberWidth;
    src.h = numberHeight;

    SDL_Rect dest;
    dest.x = (m_x * spritesheet->getTileWidth() + 8) * scale;
    dest.y = (m_y * spritesheet->getTileHeight() + 12) * scale;
    dest.w = numberWidth * scale;
    dest.h = numberHeight * scale;

    SDL_RenderCopy(engine.renderer(), numbers, &src, &dest);

    if (hp == 10)
    {
        src.x = 8;

        dest.x = (m_x * spritesheet->getTileWidth() + 1) * scale;

        SDL_RenderCopy(engine.renderer(), numbers, &src, &dest);
    }
}

UnitFaction Unit::getFaction()
{
    return this->m_faction;
}

void Unit::setState(UnitState state)
{
    this->m_state = state;
}

bool Unit::hasAttacked()
{
    return this->m_hasAttacked;
}

bool Unit::hasMoved()
{
    return this->m_hasMoved;
}

int Unit::getXPosition()
{
    return m_x;
}

int Unit::getYPosition()
{
    return m_y;
}

int Unit::getMovementPoints()
{
    return m_movementPoints;
}
MovementType Unit::getMovementType()
{
    return m_movementType;
}

} // namespace advanced_wars
