#include "Unit.hpp"
#include "Box2dHelper.hpp"
#include "Bullet.hpp"
#include "UnitContactListener.hpp"
#include <iostream>

namespace advanced_wars
{

Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state, b2World* world = nullptr)
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

        dst.x = ((worldToTile(pos.x) * spritesheet->getUnitWidth()) - 4) * scale;
        dst.y = ((worldToTile(pos.y) * spritesheet->getUnitHeight()) - 4) * scale;
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