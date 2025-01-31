#pragma once

#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "ui/contextmenu.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace advanced_wars
{

/**
 * @brief The main window of the game
 */
class Level : public Scene
{
    public:
        Level(
            std::string name, int width, int height, std::vector<Tile> tiles,
            std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect>);

        static Level loadLevel(std::string path);

        void render(Engine* engine);

        void handleEvent(Engine* engine, SDL_Event& event);

        void handleEvent2(Engine* engine, SDL_Event& event);

        int add_building(Building building);

        Building remove_building(int id);

        int add_unit(Unit unit);

        Unit remove_unit(int id);

        int add_effect(Effect effect);

        Effect remove_effect(int id);

        void handleEvent(Engine& engine, SDL_Event& event);

    private:
        std::string name;
        int         width;
        int         height;

        std::vector<Tile>                 tiles;
        std::unordered_map<int, Building> buildings;
        std::unordered_map<int, Unit>     units;
        std::unordered_map<int, Effect>   effects;
        int                               selectedUnit;
        int                               targetedUnit;
        int                               selectedBuilding;
        bool                              selectUnit(int tileX, int tileY);
        bool                              target_unit(int tileX, int tileY);
        bool                              selectBuilding(int tileX, int tileY);

        bool click_check_left(int mouseX, int mouseY);
        bool click_check_right(int mouseX, int mouseY);

        ContextMenu context_menu;
        bool        context_menu_active;

        int id;
};

} // namespace advanced_wars
