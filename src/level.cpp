#include "level.hpp"
#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "spritesheet.hpp"
#include "ui/contextmenu.hpp"
#include "ui/pausemenu.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <string>

namespace advanced_wars
{

Level::Level(
    std::string name, int width, int height, std::vector<Tile> tiles,
    std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects)
    : name(name), width(width), height(height), tiles(tiles), context_menu(ContextMenu()),
      context_menu_active(false), id(0)
{

    context_menu.setOptions({"Move", "Info", "Wait"});

    for (Building building : buildings)
    {
        this->add_building(building);
    }

    for (Unit unit : units)
    {
        this->add_unit(unit);
    }

    for (Effect effect : effects)
    {
        this->add_effect(effect);
    }

    if ((size_t)(width * height) != tiles.size())
    {
        throw std::runtime_error("level tile mismatch");
    }
};

const int RENDERING_SCALE = 3;

bool Level::click_check_left(int tileX, int tileY)
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

bool Level::click_check_right(int tileX, int tileY)
{

    if (target_unit(tileX, tileY))
    {
        return true;
    }

    return false;
}

bool Level::selectUnit(int tileX, int tileY)
{

    // std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
    for (auto& [id, unit] : units)
    {

        if (unit.x == tileX && unit.y == tileY)
        {
            // std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;

            selectedUnit = id;
            return true;
        }
    }

    return false;
}

bool Level::target_unit(int tileX, int tileY)
{

    // std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
    for (auto& [id, unit] : units)
    {

        if (unit.x == tileX && unit.y == tileY)
        {
            // std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;

            targetedUnit = id;
            return true;
        }
    }

    return false;
}

bool Level::selectBuilding(int tileX, int tileY)
{

    for (auto& [id, building] : buildings)
    {

        if (building.x == tileX && building.y == tileY)
        {
            // std::cout << "X:" << unit.x << "Y:" << unit.y << std::endl;
            selectedBuilding = id;
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

        if (event.button.button == SDL_BUTTON_LEFT)
        {

            int tileX = event.button.x / (16 * RENDERING_SCALE);
            int tileY = event.button.y / (16 * RENDERING_SCALE);

            if (click_check_left(tileX, tileY))
            {

                if (selectedUnit > -1)
                {
                    units.at(selectedUnit).on_left_click(event);
                }

                if (selectedBuilding > -1)
                {
                    // building stuff
                }
            }
            else
            {

                std::cout << "Neither building nor unit clicked!" << std::endl;
                selectedUnit = -1;
                selectedBuilding = -1;
            }
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {

            if (selectedUnit > -1)
            {

                int tileX = event.button.x / (16 * RENDERING_SCALE);
                int tileY = event.button.y / (16 * RENDERING_SCALE);

                if (click_check_right(tileX, tileY))
                {

                    units.at(selectedUnit).attack(&(units.at(targetedUnit)));

                    if (units.at(selectedUnit).health <= 0)
                    {
                        remove_unit(selectedUnit);
                    }
                }
                else
                {

                    units.at(selectedUnit).update_position(tileX, tileY);
                }
            }
            else
            {

                std::cout << "No unit selected! " << std::endl;
            }
        }
    }
}

void Level::render(Engine* engine)
{

    // Iterate over all events
    while (!engine->events().empty())
    {
        // handleEvent(engine, engine->events().at(0));
        handleEvent(*engine, engine->events().at(0));
        engine->events().pop_front();
    }

    // Tiles
    for (Tile& tile : tiles)
    {
        tile.render(engine, RENDERING_SCALE);
    }

    // Buildings
    for (auto& [id, building] : buildings)
    {
        building.render(engine, RENDERING_SCALE);
    }

    // Units
    for (auto& [id, unit] : units)
    {
        unit.render(engine, RENDERING_SCALE);
    }

    // Effects
    std::vector<int> effects_to_remove;
    for (auto& [id, effect] : effects)
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
        this->remove_effect(id);
    }

    if (context_menu_active)
    {
        context_menu.render(engine);
    }
}

void Level::handleEvent(Engine* engine, SDL_Event& event)
{
    // Handle events for the level
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            // Pause the game
            std::cout << "Pausing game..." << std::endl;
            SDL_Texture* currentTexture = SDL_CreateTexture(
                engine->renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

            PauseMenu pauseMenu(0, currentTexture);
            engine->push_scene(std::make_shared<PauseMenu>(pauseMenu));
        }
        if (context_menu_active)
        {
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                context_menu.handleEvent(event);
            }
            if (event.key.keysym.sym == SDLK_UP)
            {
                context_menu.handleEvent(event);
            }
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                if (context_menu.getSelectedOption() == "Wait")
                {
                    context_menu_active = false;
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        context_menu.update(event.button.x, event.button.y);
        context_menu_active = true;
    }
}

int Level::add_building(Building building)
{
    buildings.insert({id, building});
    id += 1;

    return id - 1;
}

Building Level::remove_building(int id)
{
    Building value = buildings.at(id);
    buildings.erase(id);

    return value;
}

int Level::add_unit(Unit unit)
{
    units.insert({id, unit});
    id += 1;

    return id - 1;
}

Unit Level::remove_unit(int id)
{
    Unit value = units.at(id);
    units.erase(id);

    return value;
}

int Level::add_effect(Effect effect)
{
    effects.insert({id, effect});
    id += 1;

    return id - 1;
}

Effect Level::remove_effect(int id)
{
    Effect value = effects.at(id);
    effects.erase(id);

    return value;
}

} // namespace advanced_wars