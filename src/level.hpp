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

        void render(Engine& engine);

        void handleEvent(Engine& engine, SDL_Event& event);

        int addBuilding(Building building);

        Building removeBuilding(int id);

        int addUnit(Unit unit);

        Unit removeUnit(int id);

        int addEffect(Effect effect);

        Effect removeEffect(int id);

    private:
        std::string m_name;
        int         m_width;
        int         m_height;

        std::vector<Tile>                 m_tiles;
        std::unordered_map<int, Building> m_buildings;
        std::unordered_map<int, Unit>     m_units;
        std::unordered_map<int, Effect>   m_effects;

        int m_selectedUnit;
        int m_targetedUnit;
        int m_selectedBuilding;

        ContextMenu m_contextMenu;
        bool        m_contextMenuActive;

        int m_id;

        bool selectUnit(int tileX, int tileY);
        bool targetUnit(int tileX, int tileY);
        bool selectBuilding(int tileX, int tileY);

        bool clickCheckLeft(int mouseX, int mouseY);
        bool clickCheckRight(int mouseX, int mouseY);
};

} // namespace advanced_wars
