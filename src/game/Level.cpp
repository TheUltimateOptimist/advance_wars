#include "Level.hpp"
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
#include <iostream>
#include <string>

namespace advanced_wars
{

const int RENDERING_SCALE = 3;

Level::Level(
    std::string name, int width, int height, std::vector<Tile> tiles,
    std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects)
    : m_name(name), m_width(width), m_height(height), m_tiles(tiles), m_selectedUnit(-1),
      m_selectedBuilding(-1), m_contextMenu(ContextMenu()), m_id(0),
      m_state(LevelState::SELECTING_STATE)
{

    m_contextMenu.setOptions({"Move", "Info", "Wait"});

    for (Building building : buildings)
    {
        this->addBuilding(building);
    }

    for (Unit unit : units)
    {
        this->addUnit(unit);
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

std::pair<int, int> Level::calcTilePos(int mouseX, int mouseY)
{
    int tileSize = (16 * RENDERING_SCALE);
    int tileX = mouseX / tileSize;
    int tileY = mouseY / tileSize;

    return {tileX, tileY};
}

void Level::selectEntity(int x, int y)
{
    std::pair<int, int> tilePos = calcTilePos(x, y);

    if (!(m_selectedUnit = selectUnit(tilePos.first, tilePos.second)))
    {
        return;
    }
    if (!(m_selectedBuilding = selectBuilding(tilePos.first, tilePos.second)))
    {
        return;
    }
}

int Level::selectUnit(int tileX, int tileY)
{
    for (auto& [id, unit] : m_units)
    {
        if (unit.m_x == tileX && unit.m_y == tileY)
        {
            return id;
        }
    }
    return -1;
}

int Level::selectBuilding(int tileX, int tileY)
{
    for (auto& [id, building] : m_buildings)
    {
        if (building.m_x == tileX && building.m_y == tileY)
        {
            return id;
        }
    }
    return -1;
}

void Level::handleEvent(Engine& engine, SDL_Event& event)
{

    /*
    on event
        key down
            escape      ->  deselect/ open pause menu
            key left    ->  move one tile left
            key right   ->  move one tile right
            key up      ->  move one tile up / change context menu selection up
            key down    ->  move one tile down / change context menu selection down
            key enter   ->  confirm selection in context menu / confirm selected position (moving)/
                            select entity on tile
        mousebutton down
            button left ->  select field/building/unit / move to position
            button right->  not neccessarliy needed

        two states: unit/building selected/ nothing selected

    */

    switch (m_state)
    {
    case LevelState::MENUACTIVE_STATE:
        /*
        Escape
        Key Down
        Key up
        Enter
         */
        handleMenuActiveEvents(engine, event);
        break;
    case LevelState::SELECTING_STATE:
        handleSelectingEvents(engine, event);
        break;
    case LevelState::ANIMATING_STATE:
        // maybe do nothing
        break;
    case LevelState::MOVEMENT_STATE:
        /*
        key left right up down
        escape
        enter
        */
        break;
    default:
        break;
    }

    /*switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:

        // the current unit debug combat should be handled by the contextmenu with its menu options
        if (event.button.button == SDL_BUTTON_LEFT)
        {

            int tileX = event.button.x / (16 * RENDERING_SCALE);
            int tileY = event.button.y / (16 * RENDERING_SCALE);

            if (clickCheckLeft(tileX, tileY))
            {

                if (m_selectedUnit > -1)
                {
                    m_units.at(m_selectedUnit).on_left_click(event);
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

                    m_units.at(m_selectedUnit).attack((m_units.at(m_targetedUnit)));

                    if (m_units.at(m_selectedUnit).m_health <= 0)
                    {
                        removeUnit(m_selectedUnit);
                    }
                }
                else
                {

                    m_units.at(m_selectedUnit).updatePosition(tileX, tileY);
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
    }*/
}

void Level::render(Engine& engine)
{
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
        unit.render(engine, RENDERING_SCALE);
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

    if (m_state == LevelState::MENUACTIVE_STATE)
    {
        m_contextMenu.render(engine);
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

int Level::addUnit(Unit unit)
{
    m_units.insert({m_id, unit});
    m_id += 1;

    return m_id - 1;
}

Unit Level::removeUnit(int id)
{
    Unit value = m_units.at(id);
    m_units.erase(id);

    return value;
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

void Level::handleSelectingEvents(Engine& engine, SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            engine.pushScene(std::make_shared<PauseMenu>(0, nullptr));
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            selectEntity(event.button.x, event.button.y);
            if (m_selectedUnit >= 0 || m_selectedBuilding >= 0)
            {
                std::pair<int, int> tilePos = calcTilePos(event.button.x, event.button.y);
                m_contextMenu.update(
                    (tilePos.first * 16 + 15) * RENDERING_SCALE,
                    (tilePos.second * 16 + 15) * RENDERING_SCALE);
                m_state = LevelState::MENUACTIVE_STATE;
            }
            else
            {
                m_state = LevelState::SELECTING_STATE;
            }
        }
    default:
        break;
    }
}

void Level::handleMenuActiveEvents(Engine& engine, SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            m_selectedUnit = -1;
            m_selectedBuilding = -1;
            m_state = LevelState::SELECTING_STATE;
        }
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
        {
            m_contextMenu.handleEvent(engine, event);
        }
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            if (m_contextMenu.getSelectedOption() == "Wait")
            {
                m_state = LevelState::SELECTING_STATE;
            }
        }

        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
        }
    default:
        break;
    }
}

} // namespace advanced_wars
