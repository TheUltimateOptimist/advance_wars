#include "Unit.hpp"
#include "Box2dHelper.hpp"
#include "Bullet.hpp"
#include "Config.hpp"
#include "UnitContactListener.hpp"
#include <iostream>

namespace advanced_wars
{

Unit::Unit(
    int x, int y, UnitFaction faction, UnitId id, UnitState state, Config& config,
    b2World* world = nullptr)
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

void Unit::setWorld(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // Positioniere den Body so, dass sein Zentrum in der Mitte des Tiles liegt:
    float posX = (this->getX() * 16 + 8) / PIXELS_PER_METER;
    float posY = (this->getY() * 16 + 8) / PIXELS_PER_METER;
    bodyDef.position.Set(posX, posY);

    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape hitboxShape;
    // Erstelle ein Rechteck von 16x16 Pixeln (halbe Breite = 8 Pixel, umgerechnet in Meter)
    hitboxShape.SetAsBox((16.0f / 2.0f) / PIXELS_PER_METER, (16.0f / 2.0f) / PIXELS_PER_METER);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &hitboxShape;
    fixtureDef.isSensor = true;

    m_body->CreateFixture(&fixtureDef);

    // User-Daten setzen
    BodyUserData* bud = new BodyUserData();
    bud->type = BodyUserData::Type::Unit;
    bud->data = this;
    bud->uniqueId = this->getMapId();
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(bud);
}

void Unit::render(Engine& engine, int scale)
{
    b2Vec2 pos = m_body->GetPosition();

    Spritesheet* spritesheet = engine.getSpritesheet();

    int step = engine.getStage() % spritesheet->getUnitTextures()
                                       .at(static_cast<int>(m_faction))
                                       .at(static_cast<int>(m_id))
                                       .at(static_cast<int>(m_state))
                                       .second;
    SDL_Rect src;
    SDL_Rect dst;
    if (m_state == UnitState::IDLE || m_state == UnitState::UNAVAILABLE)
    {

        src.x = step * spritesheet->getUnitWidth();
        src.y = 0;
        src.w = spritesheet->getUnitWidth();
        src.h = spritesheet->getUnitHeight();

        dst.x = worldToTile(pos.x) * spritesheet->getUnitWidth() * scale;
        dst.y = worldToTile(pos.y) * spritesheet->getUnitHeight() * scale;
        dst.w = spritesheet->getUnitWidth() * scale;
        dst.h = spritesheet->getUnitHeight() * scale;
    }
    else
    {
        // The moving states have a resolution of 24x24 instead of 16x16 and need to
        // be handled separately
        src.x = step * spritesheet->getUnitMovingWidth();
        src.y = 0;
        src.w = spritesheet->getUnitMovingWidth();
        src.h = spritesheet->getUnitMovingHeight();

        SDL_Rect dst;
        dst.x = ((worldToTile(pos.x) * spritesheet->getUnitWidth()) - 4) * scale;
        dst.y = ((worldToTile(pos.y) * spritesheet->getUnitHeight()) - 8) * scale;
        dst.w = spritesheet->getUnitMovingWidth() * scale;
        dst.h = spritesheet->getUnitMovingHeight() * scale;
    }

    SDL_RenderCopyEx(
        engine.renderer(),
        spritesheet->getUnitTextures()
            .at(static_cast<int>(m_faction))
            .at(static_cast<int>(m_id))
            .at(static_cast<int>(m_state))
            .first,
        &src, &dst, 0, NULL, SDL_FLIP_NONE);
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
    auto primary_damage_it = primaryWeapon->m_damage.find(target.m_id);
    auto secondary_damage_it = secondaryWeapon->m_damage.find(target.m_id);

    int damage_value = 0;

    // Calculate damage using secondary weapon if available
    if (secondary_damage_it != secondaryWeapon->m_damage.end())
    {
        damage_value = secondary_damage_it->second;
    }

    // Calculate damage using primary weapon if higher and ammo is available
    if (primary_damage_it != primaryWeapon->m_damage.end())
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
        if (unit == this)
        {
            continue;
        }

        int distanceX = std::abs(unit->m_x - m_x);
        int distanceY = std::abs(unit->m_y - m_y);

        int distance = distanceX + distanceY;
        if (distance >= m_minRange && distance <= m_maxRange)
        {
            // Prüfen ob Schaden möglich ist
            auto primaryDamageIt = m_primaryWeapon.m_damage.find(unit->m_id);
            auto secondaryDamageIt = m_secondaryWeapon.m_damage.find(unit->m_id);

            bool canDealDamage = false;

            // Prüfen, ob Primärwaffe Schaden machen kann
            if (primaryDamageIt != m_primaryWeapon.m_damage.end() && m_ammo > 0)
            {
                canDealDamage = true;
            }
            // Prüfen, ob Sekundärwaffe Schaden machen kann
            if (secondaryDamageIt != m_secondaryWeapon.m_damage.end())
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

int Unit::getX() const
{
    return this->m_x;
}

int Unit::getY() const
{
    return this->m_y;
}

int Unit::getHealth() const
{
    return this->m_health;
}

int Unit::getId() const
{
    return static_cast<int>(this->m_id);
}

float Unit::getPixelX()
{
    return getX() * 16 * 3;
}

float Unit::getPixelY()
{
    return getY() * 16 * 3;
}

void Unit::setMapId(int id)
{
    this->m_mapId = id;
}

int Unit::getMapId()
{
    return this->m_mapId;
}

void Unit::moveToTile(int targetX, int targetY)
{
    // Speichere die Ziel-Tile-Koordinaten
    m_targetTileX = targetX;
    m_targetTileY = targetY;

    // Zielposition in Meter umrechnen (Mitte des Tiles)
    float worldTargetX = (targetX * 16 + 8) / PIXELS_PER_METER;
    float worldTargetY = (targetY * 16 + 8) / PIXELS_PER_METER;

    // Aktuelle Position abrufen
    b2Vec2 currentPos = m_body->GetPosition();

    // Differenz berechnen
    float deltaX = worldTargetX - currentPos.x;
    float deltaY = worldTargetY - currentPos.y;

    // Distanz berechnen
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    if (distance < 0.1f)
    {
        return; // Falls schon fast da, nichts tun
    }

    // Normierte Richtung berechnen
    float directionX = deltaX / distance;
    float directionY = deltaY / distance;

    // Geschwindigkeit setzen
    float speed = 2.0f; // Tiles pro Sekunde
    m_body->SetLinearVelocity(b2Vec2(directionX * speed, directionY * speed));

    // State setzen
    calcState(targetX, targetY);
}

void Unit::update()
{
    b2Vec2 pos = m_body->GetPosition();

    // Berechne aktuelle Tile-Position
    int currentTileX = static_cast<int>((pos.x * PIXELS_PER_METER) / 16);
    int currentTileY = static_cast<int>((pos.y * PIXELS_PER_METER) / 16);
    /*  if (getMapId() == 66)
     {
         std::cout << "Current Tile: " << currentTileX << ", " << currentTileY << std::endl;
     } */
    // Prüfe, ob wir am Ziel sind
    if (currentTileX == m_targetTileX && currentTileY == m_targetTileY)
    {
        m_body->SetLinearVelocity(b2Vec2(0, 0)); // Bewegung stoppen
        m_x = m_targetTileX; // Stelle sicher, dass die Unit auf dem richtigen Tile registriert
                             // ist
        m_y = m_targetTileY;
        m_state = UnitState::IDLE;
    }
}

} // namespace advanced_wars