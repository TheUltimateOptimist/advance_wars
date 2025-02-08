#include "Level.hpp"
#include "../core/Config.hpp"
#include "../core/Engine.hpp"
#include "../core/Spritesheet.hpp"
#include "../effect/Effect.hpp"
#include "../entities/Building.hpp"
#include "../entities/Unit.hpp"
#include "../ui/context/ContextMenu.hpp"
#include "../ui/menu/EndScreen.hpp"
#include "../ui/menu/PauseMenu.hpp"
#include "../ui/modals/HelpMenu.hpp"

#include <SDL.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <highfive/H5File.hpp>
#include <iostream>
#include <string>

namespace advanced_wars
{

const int RENDERING_SCALE = 3;

Level::Level(
    std::string name, int width, int height, std::vector<Tile> tiles,
    std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects,
    std::queue<Player> turnQ)
    : m_name(name), m_width(width), m_height(height), m_tiles(tiles), m_selectedUnit(-1),
      m_selectedBuilding(-1), m_contextMenu(ContextMenu()), m_id(0),
      m_state(LevelState::SELECTING_STATE),
      m_currentPos(TileMarker(RENDERING_SCALE, 1, 1, m_width, m_height)), m_turnQ(turnQ),
      m_gameOver(false)
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

    m_selectedBuilding = -1;
    m_selectedUnit = -1;
};

std::shared_ptr<Level> Level::loadLevel(const std::string& path, Engine& engine)
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

    // if level is smaler than 20x20 surround with water tiles
    if (width < 20 || height < 20)
    {
        int                  w_start = (20 - width) / 2;
        int                  h_start = (20 - height) / 2;
        std::vector<uint8_t> transformed_tiles_array;
        transformed_tiles_array.reserve(20 * 20);
        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                if (x < w_start || y < h_start || x >= w_start + width || y >= h_start + height)
                {
                    transformed_tiles_array.push_back(1);
                }
                else
                {
                    transformed_tiles_array.push_back(
                        level_tilesarray[x - w_start + (y - h_start) * width]);
                }
            }
        }
        level_tilesarray = std::move(transformed_tiles_array);
        width = 20;
        height = 20;
    }

    // create tiles, buildings and units vector from tiles array
    std::vector<Tile>     tiles;
    std::vector<Building> buildings;
    std::vector<Unit>     units;
    tiles.reserve(width * height);
    bool has_factions[] = {false, false, false, false, false};
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
            if (building_id == BuildingId::HEADQUARTER)
            {
                int index = static_cast<int>(faction_id);
                if (!has_factions[index])
                {
                    units.push_back(Unit(
                        x, y, static_cast<UnitFaction>(faction_id), UnitId::INFANTERY,
                        UnitState::UNAVAILABLE, engine.getUnitConfig()));
                }
                has_factions[static_cast<int>(faction_id)] = true;
            }
            buildings.push_back(Building(x, y, building_id, faction_id));
        }
        else
        {
            TileId tile_id = static_cast<TileId>(level_tilesarray[i]);
            tiles.push_back(Tile(tile_id, x, y));
        }
    }

    // create turnQ from has_factions array
    std::queue<Player> turnQ;
    for (int i = 0; i < 5; i++)
    {
        if (has_factions[i])
        {
            turnQ.push(Player(2000, static_cast<UnitFaction>(i)));
        }
    }

    Level level(name, width, height, tiles, buildings, units, std::vector<Effect>{}, turnQ);

    level.m_turnQ.front().startTurn(level.m_units, level.m_buildings);
    std::cout << "exiting" << std::endl;
    return std::make_shared<Level>(level);
}

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

    if ((m_selectedUnit = selectUnit(tilePos.first, tilePos.second)) >= 0)
    {
        auto it = m_units.find(m_selectedUnit);
        if (it != m_units.end())
        {
            Unit& unit = it->second;
            m_unitInfoMenu.setUnit(unit);
            // Position das Menu rechts neben der ausgewählten Einheit
            m_unitInfoMenu.update(
                (tilePos.first * 16 + 20) * RENDERING_SCALE, // x-Position
                (tilePos.second * 16) * RENDERING_SCALE      // y-Position
            );
        }
        return;
    }
    if ((m_selectedBuilding = selectBuilding(tilePos.first, tilePos.second)) >= 0)
    {
        // Optionale Handhabung für Gebäude
        return;
    }
}

int Level::selectUnit(int tileX, int tileY)
{
    for (auto& [id, unit] : m_units)
    {
        if (unit.getXPosition() == tileX && unit.getYPosition() == tileY)
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
        if (building.getXPosition() == tileX && building.getYPosition() == tileY)
        {
            return id;
        }
    }
    return -1;
}

void Level::handleEvent(Engine& engine, SDL_Event& event)
{
    if (m_gameOver)
    {
        engine.pushScene(std::make_shared<Endscreen>(Endscreen(m_turnQ.front())));
        return;
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_h)
        {
            toggle_Helpmenu = !toggle_Helpmenu;
        }
    }
    switch (m_state)
    {
    case LevelState::MENUACTIVE_STATE:
        handleMenuActiveEvents(engine, event);
        break;
    case LevelState::SELECTING_STATE:
        handleSelectingEvents(engine, event);
        break;
    case LevelState::ANIMATING_STATE:
        // maybe do nothing
        break;
    case LevelState::MOVEMENT_STATE:
        handleMovementEvents(engine, event);
        break;
    case LevelState::ATTACKING_STATE:
        handleAttackingEvents(engine, event);
        break;
    case LevelState::RECRUITING_STATE:
        handleRecruitingEvent(engine, event);
        break;
    default:
        break;
    }
}

std::vector<std::pair<int, int>> Level::calculateMovementRange(Unit& unit)
{
    std::vector<std::pair<int, int>>      reachableTiles;
    std::queue<std::tuple<int, int, int>> wavefrontQueue; // x, y, remainingMovement

    wavefrontQueue.push(
        std::make_tuple(unit.getXPosition(), unit.getYPosition(), unit.getMovementPoints()));
    std::unordered_map<int, std::unordered_map<int, bool>> visited;

    while (!wavefrontQueue.empty())
    {
        auto [x, y, remainingMovement] = wavefrontQueue.front();
        wavefrontQueue.pop();

        // Check if this tile has been visited already
        if (visited[x][y])
            continue;
        visited[x][y] = true;

        // Check if a unit is on the current tile, skip adding it if true
        bool isOccupied = false;
        for (auto& [id, otherUnit] : m_units)
        {
            if (otherUnit.getXPosition() == x && otherUnit.getYPosition() == y &&
                id != m_selectedUnit)
            {
                isOccupied = true;
                break;
            }
        }
        // Add to reachable tiles only if not occupied
        if (!isOccupied)
        {
            reachableTiles.emplace_back(x, y);
        }

        static const std::vector<std::pair<int, int>> directions = {
            { 1,  0},
            {-1,  0},
            { 0,  1},
            { 0, -1}
        };

        for (const auto& [dx, dy] : directions)
        {
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height)
                continue; // Boundary check

            int cost = getMoveCost(m_tiles[ny * m_width + nx].getType(), unit.getMovementType());
            if (cost >= 0 && remainingMovement >= cost)
            {
                wavefrontQueue.push(std::make_tuple(nx, ny, remainingMovement - cost));
            }
        }
    }

    return reachableTiles;
}

int Level::getMoveCost(TileId tileId, MovementType movementType)
{
    return moveCostTable[static_cast<int>(tileId)][static_cast<int>(movementType)];
}

void Level::render(Engine& engine)
{
    // Tiles
    for (Tile& tile : m_tiles)
    {
        tile.render(engine, RENDERING_SCALE);
    }

    if (m_showReachableTiles)
    {
        SDL_SetRenderDrawColor(engine.renderer(), 255, 255, 0, 128); // Gelb mit leichtem Alpha

        for (const auto& [x, y] : m_reachableTiles)
        {
            SDL_Rect rect = {
                x * 16 * RENDERING_SCALE, y * 16 * RENDERING_SCALE, 16 * RENDERING_SCALE,
                16 * RENDERING_SCALE};
            SDL_RenderFillRect(engine.renderer(), &rect);
        }

        SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 0, 255);
    }

    if (m_showAttackableTiles)
    {
        SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 128); // Rot mit leichtem Alpha

        for (const auto& [x, y] : m_attackableTiles)
        {
            SDL_Rect rect = {
                x * 16 * RENDERING_SCALE, y * 16 * RENDERING_SCALE, 16 * RENDERING_SCALE,
                16 * RENDERING_SCALE};
            SDL_RenderFillRect(engine.renderer(), &rect);
        }

        SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 0, 255);
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

    if (m_state == LevelState::RECRUITING_STATE)
    {
        m_recruitingMenu.render(engine);
    }

    if (m_showUnitInfoMenu)
    {
        m_unitInfoMenu.render(engine);
    }

    m_currentPos.render(engine);

    if (toggle_Helpmenu)
    {
        m_helpMenu.render(engine);
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

void Level::changeTurn()
{
    Player temp = m_turnQ.front();

    temp.endTurn(m_units);

    // Cycle Player at end of queue
    m_turnQ.pop();
    m_turnQ.push(temp);

    m_turnQ.front().startTurn(m_units, m_buildings);
    m_currentPos.setMarkerColor(m_turnQ.front().getFaction());
}

void Level::handleRecruitingEvent(Engine& engine, SDL_Event& event)
{

    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            m_state = LevelState::MENUACTIVE_STATE;
        }
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
        {
            m_recruitingMenu.handleEvent(engine, event);
        }
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            Building&   b = m_buildings.at(m_selectedBuilding);
            UnitFaction u_faction = static_cast<UnitFaction>(b.getFaction());
            UnitId      unit_id = m_recruitingMenu.getSelectedOption();
            int         cost = engine.getUnitConfig().getUnitCost(unit_id);

            if (b.check_money(cost, m_turnQ.front().getMoney()))
            {
                if (b.check_spawn(m_units))
                {
                    addUnit(Unit(
                        b.getXPosition(), b.getYPosition(), u_faction, unit_id, UnitState::IDLE,
                        engine.getUnitConfig()));
                    m_state = LevelState::SELECTING_STATE;
                    m_turnQ.front().spendMoney(cost);
                    m_selectedBuilding = -1;
                }
            }
            else
            {
                std::cout << "You dont have enough money, current money: "
                          << m_turnQ.front().getMoney() << " || needed money: " << cost
                          << std::endl;
            }
        }
    }
}

//*******************helper functions for event Handling*************************************

void Level::handleAttack(std::pair<int, int> tilePos)
{
    int targetedUnit = selectUnit(tilePos.first, tilePos.second);
    if (targetedUnit >= 0)
    {
        if (m_units.at(m_selectedUnit).getFaction() == m_units.at(targetedUnit).getFaction())
        {
            std::cout << "You cannot attack your allies!" << std::endl;
            return;
        }

        Unit& attacking = m_units.at(m_selectedUnit);
        Unit& defending = m_units.at(targetedUnit);

        if (m_attackableUnitIds.find(targetedUnit) != m_attackableUnitIds.end())
        {
            attacking.attack(defending);
            if (attacking.getHealth() <= 0)
            {
                removeUnit(m_selectedUnit);
            }
            else
            {
                attacking.setState(UnitState::UNAVAILABLE);
            }
            if (defending.getHealth() <= 0)
            {
                removeUnit(targetedUnit);
            }
            m_selectedUnit = -1;
            m_showAttackableTiles = false;
            m_showReachableTiles = false;
            m_state = LevelState::SELECTING_STATE;
        }
        else
        {
            std::cout << "No target in range clicked!" << std::endl;
        }
    }
    else
    {
        std::cout << "No valid target clicked" << std::endl;
    }
}

void Level::handleMovement(std::pair<int, int> tilePos)
{
    for (auto& [id, unit] : m_units)
    {
        if (unit.getXPosition() == tilePos.first && unit.getYPosition() == tilePos.second)
        {
            // unit already at clicked position (maybe even selected unit)
            std::cout << "Unit already at clicked position" << std::endl;
            return;
        }
    }

    bool isReachable = false;

    for (auto& pos : m_reachableTiles)
    {
        if (pos == tilePos)
        {
            isReachable = true;
            break;
        }
    }

    if (isReachable)
    {
        m_units.at(m_selectedUnit).updatePosition(tilePos.first, tilePos.second);

        m_contextMenu.update(
            (tilePos.first * 16 + 15) * RENDERING_SCALE,
            (tilePos.second * 16 + 15) * RENDERING_SCALE);

        std::vector<Unit*> allUnits;

        for (auto& [id, unit] : m_units)
        {
            allUnits.push_back(&unit);
        }

        std::vector<Unit*> attackableTargets =
            m_units.at(m_selectedUnit).getUnitsInRangeWithDamagePotential(allUnits);

        m_attackableTiles.clear();
        m_showAttackableTiles = true;
        m_attackableUnitIds.clear();

        for (Unit* target : attackableTargets)
        {
            // Füge die Position jedes angreifbaren Ziels hinzu
            m_attackableTiles.emplace_back(target->getXPosition(), target->getYPosition());

            // Angreifbaren Einheits-ID setzen
            for (auto& [id, unit] : m_units)
            {
                if (&unit == target)
                {
                    m_attackableUnitIds.insert(id);
                    break;
                }
            }
        }

        m_showReachableTiles = false;

        m_contextMenu.setOptions({"Attack", "Wait", "End Turn"});

        m_state = LevelState::MENUACTIVE_STATE;
    }
    else
    {
        std::cout << "Unglültige Bewegunsposition!" << std::endl;
    }
}

void Level::handlePositionMarker(Engine& engine, SDL_Event& event)
{
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN ||
        event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
    {
        m_currentPos.handleEvent(engine, event);
    }
}

//*******************end helper functions for event Handling*********************************

//************event handler delegates for different level states*****************************

void Level::handleSelectingEvents(Engine& engine, SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        handlePositionMarker(engine, event);
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            engine.pushScene(std::make_shared<PauseMenu>(0, nullptr));
        }

        if (event.key.keysym.sym == SDLK_RETURN)
        {
            std::pair<int, int> tilePos = m_currentPos.getPosition();
            selectEntity(
                tilePos.first * 16 * RENDERING_SCALE, tilePos.second * 16 * RENDERING_SCALE);
            if (m_selectedUnit >= 0 || m_selectedBuilding >= 0)
            {
                m_contextMenu.update(
                    (tilePos.first * 16 + 15) * RENDERING_SCALE,
                    (tilePos.second * 16 + 15) * RENDERING_SCALE);
                if (m_selectedUnit >= 0)
                {
                    m_reachableTiles = calculateMovementRange(m_units.at(m_selectedUnit));
                    m_units.at(m_selectedUnit).on_left_click(event);
                    m_showReachableTiles = true;

                    std::vector<Unit*> allUnits;

                    for (auto& [id, unit] : m_units)
                    {
                        allUnits.push_back(&unit);
                    }

                    std::vector<Unit*> attackableTargets =
                        m_units.at(m_selectedUnit).getUnitsInRangeWithDamagePotential(allUnits);

                    m_attackableTiles.clear();
                    m_showAttackableTiles = true;
                    m_attackableUnitIds.clear();

                    // Set Fallback_position if movement will be canceled
                    unit_fallback_position = std::make_pair(
                        m_units.at(m_selectedUnit).getXPosition(),
                        m_units.at(m_selectedUnit).getYPosition());

                    for (Unit* target : attackableTargets)
                    {
                        // Füge die Position jedes angreifbaren Ziels hinzu
                        m_attackableTiles.emplace_back(
                            target->getXPosition(), target->getYPosition());

                        // Angreifbaren Einheits-ID setzen
                        for (auto& [id, unit] : m_units)
                        {
                            if (&unit == target)
                            {
                                m_attackableUnitIds.insert(id);
                                break;
                            }
                        }
                    }

                    Unit& u = m_units.at(m_selectedUnit);

                    if (m_units.at(m_selectedUnit).getFaction() == m_turnQ.front().getFaction() &&
                        m_units.at(m_selectedUnit).getState() != UnitState::UNAVAILABLE)
                    {
                        m_captureBuilding = -1;
                        for (auto& [id, building] : m_buildings)
                        {
                            if (building.getXPosition() == u.getXPosition() &&
                                building.getXPosition() == u.getYPosition())
                            {
                                if (building.getFaction() !=
                                    static_cast<BuildingFaction>(u.getFaction()))
                                {
                                    m_captureBuilding = id;
                                    m_contextMenu.setOptions(
                                        {"Capture", "Move", "Attack", "Info", "Wait", "End Turn"});
                                    break;
                                }
                            }
                        }

                        if (m_captureBuilding == -1)
                        {
                            m_contextMenu.setOptions(
                                {"Move", "Attack", "Info", "Wait", "End Turn"});
                        }
                    }
                    else
                    {
                        m_contextMenu.setOptions({"Info", "End Turn"});
                    }
                    m_state = LevelState::MENUACTIVE_STATE;
                }
                else
                {
                    BuildingId      b_id = m_buildings.at(m_selectedBuilding).getId();
                    BuildingFaction b_faction = m_buildings.at(m_selectedBuilding).getFaction();
                    if (b_id == BuildingId::CITY || b_id == BuildingId::HEADQUARTER ||
                        b_faction == static_cast<BuildingFaction>(5))
                    {
                        m_contextMenu.setOptions({"Info", "End Turn"});
                    }
                    else
                    {
                        // Show according menu options if building has same/different faction than
                        // current player
                        if (m_buildings.at(m_selectedBuilding).getFaction() ==
                            static_cast<BuildingFaction>(m_turnQ.front().getFaction()))
                        {
                            m_contextMenu.setOptions({"Train", "Info", "End Turn"});
                        }
                        else
                        {
                            m_contextMenu.setOptions({"Info", "End Turn"});
                        }
                    }
                    m_state = LevelState::MENUACTIVE_STATE;
                }
            }
        }
        break;
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
            if (m_selectedUnit > -1 &&
                unit_fallback_position != std::make_pair(
                                              m_units.at(m_selectedUnit).getXPosition(),
                                              m_units.at(m_selectedUnit).getYPosition()))
            {
                m_units.at(m_selectedUnit)
                    .updatePosition(unit_fallback_position.first, unit_fallback_position.second);
            }
            m_selectedUnit = -1;
            m_selectedBuilding = -1;
            m_state = LevelState::SELECTING_STATE;
            m_showAttackableTiles = false;
            m_showReachableTiles = false;
            m_showUnitInfoMenu = false;
        }
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
        {
            if (!m_showUnitInfoMenu)
            {
                m_contextMenu.handleEvent(engine, event);
            }
        }
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            std::string cmd = m_contextMenu.getSelectedOption();
            if (cmd == "Wait")
            {
                m_state = LevelState::SELECTING_STATE;
                m_showUnitInfoMenu = false;
                auto it = m_units.find(m_selectedUnit);
                if (it != m_units.end())
                {
                    it->second.setState(UnitState::UNAVAILABLE);
                    std::cout << "Unit state set to UNAVAILABLE." << std::endl;
                    m_state = LevelState::SELECTING_STATE;
                    m_selectedUnit = -1;
                    m_selectedBuilding = -1;
                    m_showAttackableTiles = false;
                    m_showReachableTiles = false;
                }
                else
                {
                    std::cerr << "Selected unit id is invalid: " << m_selectedUnit << std::endl;
                }
            }
            if (cmd == "Move")
            {
                m_state = LevelState::MOVEMENT_STATE;
                m_showUnitInfoMenu = false;
                // Hier Pathfinding einsetzen
            }
            if (cmd == "Attack")
            {
                m_state = LevelState::ATTACKING_STATE;
                m_showUnitInfoMenu = false;
            }
            if (cmd == "Info")
            {
                // TODO: Hier Informationen zur Einheit darstellen
                if (m_selectedUnit > -1)
                {
                    Unit& u = m_units.at(m_selectedUnit);
                    std::cout << "Health: " << u.getHealth() << std::endl;
                    m_showUnitInfoMenu = !m_showUnitInfoMenu;
                }
                if (m_selectedBuilding > -1)
                {
                    Building b = m_buildings.at(m_selectedBuilding);
                    std::cout << "Building ID: " << static_cast<int>(b.getId()) << " || "
                              << "Building Faction: " << static_cast<int>(b.getFaction())
                              << std::endl;
                }
            }
            if (cmd == "Train")
            {
                m_state = LevelState::RECRUITING_STATE;
                std::pair<int, int> tilePos = m_currentPos.getPosition();
                m_recruitingMenu.update(
                    (tilePos.first * 16 + 15) * RENDERING_SCALE,
                    (tilePos.second * 16 + 15) * RENDERING_SCALE);
                m_recruitingMenu.setOptions(m_buildings.at(m_selectedBuilding).recruitableUnits());
                std::cout << "no training here" << std::endl;
            }

            if (cmd == "Capture")
            {
                Building&   b = m_buildings.at(m_captureBuilding);
                UnitFaction u_f = m_units.at(m_selectedUnit).getFaction();

                BuildingFaction b_f = static_cast<BuildingFaction>(u_f);
                m_gameOver = b.switch_faction(b_f);

                m_units.at(m_selectedUnit).setState(UnitState::UNAVAILABLE);
                m_state = LevelState::SELECTING_STATE;
                m_selectedBuilding = -1;
                m_selectedUnit = -1;
                m_showReachableTiles = false;
                m_showAttackableTiles = false;
            }
            if (cmd == "End Turn")
            {
                m_state = LevelState::SELECTING_STATE;
                m_showAttackableTiles = false;
                m_showReachableTiles = false;
                changeTurn();
                m_showUnitInfoMenu = false;
            }
        }
        break;
    default:
        break;
    }
}

void Level::handleMovementEvents(Engine& engine, SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        handlePositionMarker(engine, event);
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            handleMovement(m_currentPos.getPosition());
        }
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            m_state = LevelState::MENUACTIVE_STATE;
        }
        break;
    default:
        break;
    }
}

void Level::handleAttackingEvents(Engine& engine, SDL_Event& event)
{
    if (m_attackableUnitIds.empty())
    {
        std::cout << "No units are within attack range." << std::endl;
        m_state = LevelState::MENUACTIVE_STATE;
        return; // Early exit if no units to attack
    }
    switch (event.type)
    {
    case SDL_KEYDOWN:
        handlePositionMarker(engine, event);
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            m_state = LevelState::MENUACTIVE_STATE;
        }
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            handleAttack(m_currentPos.getPosition());
        }
        break;
    default:
        break;
    }
}
//************end event handler delegates for different level
// states*****************************

} // namespace advanced_wars
