#include "Level.hpp"
#include "Box2dHelper.hpp"
#include "Building.hpp"
#include "Effect.hpp"
#include "Engine.hpp"
#include "Spritesheet.hpp"
#include "Unit.hpp"
#include "highfive/H5File.hpp"
#include "ui/Contextmenu.hpp"
#include "ui/Pausemenu.hpp"
#include <SDL.h>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <string>

namespace advanced_wars
{

const int RENDERING_SCALE = 3;

Level::Level(
    std::string name, int width, int height, std::vector<Tile> tiles,
    std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects)
    : m_world(b2Vec2(0.0f, 0.0f)), m_contactListener(this), m_name(name), m_width(width),
      m_height(height), m_tiles(tiles), m_bullet(nullptr), m_contextMenu(ContextMenu()),
      m_contextMenuActive(false), m_id(0)
{
    m_world.SetContactListener(&m_contactListener);
    m_contextMenu.setOptions({"Move", "Info", "Wait"});

    for (Building building : buildings)
    {
        this->addBuilding(building);
    }

    for (const Unit& unit : units)
    {
        // Erstelle eine neue Instanz (Kopie) als unique_ptr
        auto newUnit = std::make_unique<Unit>(unit);
        newUnit->setWorld(&m_world);
        this->addUnit(std::move(newUnit));
    }

    for (Effect effect : effects)
    {
        this->addEffect(effect);
    }

    if ((size_t)(m_width * m_height) != tiles.size())
    {
        throw std::runtime_error("level tile mismatch");
    }
};

Level Level::loadLevel(std::string path)
{
    HighFive::File file(path, HighFive::File::ReadOnly);

    // read level metadata
    std::string level_metadata;
    file.getDataSet("metadata").read(level_metadata);

    // read tilesarray
    std::vector<uint8_t> level_tilesarray;
    file.getDataSet("tilesarray").read(level_tilesarray);

    // extract metadata from xml
    std::istringstream          xmlStream(level_metadata);
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(xmlStream, pt);
    int         width = pt.get<int>("level.width");
    int         height = pt.get<int>("level.height");
    std::string name = pt.get<std::string>("level.name");

    // create tiles and buildings vector from tiles array
    std::vector<Tile>     tiles;
    std::vector<Building> buildings;
    tiles.reserve(width * height);
    for (int i = 0; i < level_tilesarray.size(); i++)
    {
        int x = i % width;
        int y = i / width;
        if (level_tilesarray[i] >= 50)
        {
            tiles.push_back(Tile(TileId(TileId::PLAIN), x, y));
            BuildingId      building_id = static_cast<BuildingId>((level_tilesarray[i] - 50) % 5);
            BuildingFaction faction_id =
                static_cast<BuildingFaction>((level_tilesarray[i] - 50) / 5);
            buildings.push_back(Building(x, y, building_id, faction_id));
        }
        else
        {
            TileId tile_id = static_cast<TileId>(level_tilesarray[i]);
            tiles.push_back(Tile(tile_id, x, y));
        }
    }

    return Level(name, width, height, tiles, buildings, {}, {});
};

bool Level::clickCheckLeft(int tileX, int tileY)
{

    if (selectUnit(tileX, tileY))
    {
        return true;
    }

    if (selectBuilding(tileX, tileY))
    {
        return true;
    }

    return false;
}

bool Level::clickCheckRight(int tileX, int tileY)
{

    if (targetUnit(tileX, tileY))
    {
        return true;
    }

    return false;
}

bool Level::selectUnit(int tileX, int tileY)
{

    // std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
    for (auto& [id, unit] : m_units)
    {

        if (unit->m_x == tileX && unit->m_y == tileY)
        {
            // std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;

            m_selectedUnit = id;
            return true;
        }
    }

    return false;
}

bool Level::targetUnit(int tileX, int tileY)
{

    // std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
    for (auto& [id, unit] : m_units)
    {

        if (unit->m_x == tileX && unit->m_y == tileY)
        {
            // std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;

            m_targetedUnit = unit.get();
            return true;
        }
    }

    return false;
}

bool Level::selectBuilding(int tileX, int tileY)
{

    for (auto& [id, building] : m_buildings)
    {

        if (building.m_x == tileX && building.m_y == tileY)
        {
            // std::cout << "X:" << unit.x << "Y:" << unit.y << std::endl;
            m_selectedBuilding = id;
            return true;
        }
    }
    return false;
}

void Level::handleEvent(Engine& engine, SDL_Event& event)
{

    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:

        m_contextMenu.update(event.button.x, event.button.y);
        m_contextMenuActive = true;

        // the current unit debug combat should be handled by the contextmenu with its menu options
        if (event.button.button == SDL_BUTTON_LEFT)
        {

            int tileX = event.button.x / (16 * RENDERING_SCALE);
            int tileY = event.button.y / (16 * RENDERING_SCALE);

            if (clickCheckLeft(tileX, tileY))
            {

                if (m_selectedUnit > -1)
                {
                    std::cout << "Unit clicked: " << m_selectedUnit << std::endl;
                    m_units.at(m_selectedUnit)->on_left_click(event);
                }

                if (m_selectedBuilding > -1)
                {
                    // building stuff
                }
            }
            else
            {

                std::cout << "Neither building nor unit clicked!" << std::endl;
                m_selectedUnit = -1;
                m_selectedBuilding = -1;
            }
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {

            if (m_selectedUnit > -1)
            {

                int tileX = event.button.x / (16 * RENDERING_SCALE);
                int tileY = event.button.y / (16 * RENDERING_SCALE);

                if (clickCheckRight(tileX, tileY))
                {

                    m_units.at(m_selectedUnit)->attack(*m_targetedUnit);

                    if (m_units.at(m_selectedUnit)->m_health <= 0)
                    {
                        removeUnit(m_selectedUnit);
                    }
                }
                else
                {
                    m_units.at(m_selectedUnit)->moveToTile(tileX, tileY);
                }
            }
            else
            {

                std::cout << "No unit selected! " << std::endl;
            }
        }
        break;

    case SDL_KEYDOWN:

        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            // Pause the game
            std::cout << "Pausing game..." << std::endl;
            SDL_Texture* currentTexture = SDL_CreateTexture(
                engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

            PauseMenu pauseMenu(0, currentTexture);
            engine.pushScene(std::make_shared<PauseMenu>(pauseMenu));
        }
        if (m_contextMenuActive)
        {
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                if (m_contextMenu.getSelectedOption() == "Wait")
                {
                    m_contextMenuActive = false;
                }
            }
            else
            {
                m_contextMenu.handleEvent(engine, event);
            }
        }
        break;
    }
}

void Level::render(Engine& engine)
{

    // Iterate over all events
    while (!engine.events().empty())
    {
        handleEvent(engine, engine.events().at(0));

        engine.events().pop_front();
    }

    // Tiles
    for (Tile& tile : m_tiles)
    {
        tile.render(engine, RENDERING_SCALE);
    }

    // Buildings
    for (auto& [id, building] : m_buildings)
    {
        building.render(engine, RENDERING_SCALE);
    }

    // Units
    for (auto& [id, unit] : m_units)
    {
        unit->update();
        unit->render(engine, RENDERING_SCALE);
    }

    // Effects
    std::vector<int> effects_to_remove;
    for (auto& [id, effect] : m_effects)
    {
        if (effect.is_finished(engine))
        {
            effects_to_remove.push_back(id);
        }
        else
        {
            effect.render(engine, RENDERING_SCALE);
        }
    }

    // Remove finished effects after iteration
    for (int id : effects_to_remove)
    {
        this->removeEffect(id);
    }

    if (m_contextMenuActive)
    {
        m_contextMenu.render(engine);
    }

    if (m_bullet)
    {
        m_bullet->render(engine, RENDERING_SCALE);
        /* std::cout << "Bullet Position: " << m_bullet->getBody()->GetPosition().x << ", "
                   << m_bullet->getBody()->GetPosition().y << std::endl; */
    }
}

void Level::update()
{
    // Box2D-Physik-Schritt
    float timeStep = 1.0f / 60.0f;
    int   velocityIterations = 6;
    int   positionIterations = 2;

    m_world.Step(timeStep, velocityIterations, positionIterations);

    if (m_bullet)
    {
        m_bullet->update();
    }
    else
    {
        std::cout << "Bullet existiert nicht, versuche eine zu spawnen!" << std::endl; // Debug
        std::cout << m_units.size() << std::endl;
        std::cout << "Vorhandene Unit-Indizes: ";
        for (const auto& unit : m_units)
        {
            std::cout << unit.first << " "; // Falls `m_units` eine Map ist
        }
        std::cout << std::endl;
        // Prüfen, ob genügend Units existieren
        if (m_units.size() > 6)
        {
            // std::cout << "Unit Position: " << m_units.at(1).getX() << std::endl; // Debug
            spawnBullet(m_units.at(66).get(), m_units.at(71).get());
        }
        else
        {
            std::cout << "FEHLER: Nicht genug Units vorhanden!" << std::endl; // Debug
        }
    }

    if (m_removeBulletFlag)
    {
        removeBullet();
        m_removeBulletFlag = false;
    }
}

int Level::addBuilding(Building building)
{
    m_buildings.insert({m_id, building});
    m_id += 1;

    return m_id - 1;
}

Building Level::removeBuilding(int id)
{
    Building value = m_buildings.at(id);
    m_buildings.erase(id);

    return value;
}

int Level::addUnit(std::unique_ptr<Unit> unit)
{
    unit->setMapId(m_id);
    m_units.insert({m_id, std::move(unit)});
    m_id += 1;

    return m_id - 1;
}

std::unique_ptr<Unit> Level::removeUnit(int id)
{
    auto it = m_units.find(id);
    if (it == m_units.end())
    {
        return nullptr;
    }
    std::unique_ptr<Unit> removed = std::move(it->second);
    m_units.erase(it);
    return removed;
}

int Level::addEffect(Effect effect)
{
    m_effects.insert({m_id, effect});
    m_id += 1;

    return m_id - 1;
}

Effect Level::removeEffect(int id)
{
    Effect value = m_effects.at(id);
    m_effects.erase(id);

    return value;
}

void Level::spawnBullet(Unit* shooter, Unit* target)
{
    if (m_bullet)
    {
        removeBullet(); // Falls noch eine Bullet existiert, zuerst löschen
        std::cout << "Bullet deleted" << std::endl;
    }

    std::cout << "Bullet wird von Unit " << shooter->getId() << " auf Unit " << target->getId()
              << " geschossen!" << std::endl;

    std::cout << "Bullet wird gespawnt an: " << std::endl;
    std::cout << "Tile-Koordinaten (shooter): " << shooter->getX() << ", " << shooter->getY()
              << std::endl;

    // Umrechnung von Tile-Koordinaten in Pixel
    float startX = shooter->getX() * 16 + 8;
    float startY = shooter->getY() * 16 + 8;
    float targetX = target->getX() * 16 + 8;
    float targetY = target->getY() * 16 + 8;

    // Umrechnung von Pixel in Meter für Box2D
    float worldStartX = startX / PIXELS_PER_METER;
    float worldTargetX = targetX / PIXELS_PER_METER;

    float worldStartY = startY / PIXELS_PER_METER;
    float worldTargetY = targetY / PIXELS_PER_METER;

    std::cout << "Pixel-Koordinaten: " << startX << ", " << startY << std::endl;
    std::cout << "Box2D-Welt-Koordinaten: " << worldStartX << ", " << worldStartY << std::endl;

    // Distanz zwischen Start und Ziel
    float deltaX = worldTargetX - worldStartX;
    float deltaY = worldTargetY - worldStartY;

    // Gesamtstrecke berechnen (euklidische Distanz)
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

    // Normierte Richtung berechnen (Einheitsvektor)
    float directionX = deltaX / distance;
    float directionY = deltaY / distance;

    // Konstante Bullet-Geschwindigkeit (z. B. 1 m/s)
    float bulletSpeed = 1.0f;

    // Geschwindigkeit in x- und y-Richtung setzen
    float velocityX = directionX * bulletSpeed;
    float velocityY = directionY * bulletSpeed;

    std::cout << "Velocity: " << velocityX << ", " << velocityY << std::endl;

    m_bullet = new Bullet(m_world, startX, startY, velocityX, velocityY);
    m_targetedUnit = target;
}

void Level::checkBulletCollision(Unit* hitUnit)
{
    if (!m_bullet)
    {
        return;
    }
    if (hitUnit->getMapId() == m_targetedUnit->getMapId())
    {
        std::cout << "Bullet hat das Ziel getroffen!" << std::endl;

        m_removeBulletFlag = true;
    }
    else
    {
        std::cout << "Bullet fliegt weiter, hat falsches Ziel getroffen." << std::endl;
    }
}

void Level::removeBullet()
{
    if (m_bullet)
    {
        std::cout << "Bullet wird entfernt." << std::endl;
        m_world.DestroyBody(m_bullet->getBody());
        delete m_bullet;
        m_bullet = nullptr;
    }
}

void Level::markBulletForRemoval()
{
    m_removeBulletFlag = true;
}

} // namespace advanced_wars
