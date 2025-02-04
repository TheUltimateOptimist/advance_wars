#pragma once

#include "Building.hpp"
#include "Effect.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "Tile.hpp"
#include "Unit.hpp"
#include "ui/Contextmenu.hpp"
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <unordered_set>




namespace advanced_wars
{


const int NUM_TILE_IDS = 30; // Aktualisieren, falls weitere IDs hinzugefügt werden
const int NUM_MOVEMENT_TYPES = 6;


const std::array<std::array<int, NUM_MOVEMENT_TYPES>, NUM_TILE_IDS> moveCostTable = {{
    // FOOT, WHEELED, TREAD, AIR, SEA, LANDER
    {  1,      2,       1,    1, 999,   999 }, // PLAIN
    {999,    999,     999,    1,   1,     1 }, // WATER
    {  1,      3,       2,    1, 999,   999 }, // FOREST
    {  2,    999,     999,    1, 999,   999 }, // MOUNTAIN
    {  1,      1,       1,    1, 999,   999 }, // BRIDGE_HORIZONTAL
    {  1,      1,       1,    1, 999,   999 }, // STREET_HORIZONTAL
    {  1,      1,       1,    1, 999,   999 }, // STREET_VERTICAL
    {  1,      1,       1,    1, 999,   999 }, // STREET_CROSSING
    {  1,      1,       1,    1, 999,   999 }, // STREET_JUNCTION_RIGHT
    {  1,      1,       1,    1, 999,   999 }, // STREET_JUNCTION_LEFT
    {  1,      1,       1,    1, 999,   999 }, // STREET_JUNCTION_DOWN
    {  1,      1,       1,    1, 999,   999 }, // STREET_JUNCTION_UP
    {  1,      1,       1,    1, 999,   999 }, // STREET_CORNER_TOP_LEFT
    {  1,      1,       1,    1, 999,   999 }, // STREET_CORNER_TOP_RIGHT
    {  1,      1,       1,    1, 999,   999 }, // STREET_CORNER_BOTTOM_LEFT
    {  1,      1,       1,    1, 999,   999 }, // STREET_CORNER_BOTTOM_RIGHT 
    {999,    999,     999,    1,   2,     2 }, // RIFF
    {999,    999,     999,    1,   1,     1 }, // CLIFF_TOP
    {999,    999,     999,    1,   1,     1 }, // CLIFF_BOTTOM
    {999,    999,     999,    1,   1,     1 }, // CLIFF_LEFT
    {999,    999,     999,    1,   1,     1 }, // CLIFF_RIGHT
    {999,    999,     999,    1,   1,     1 }, // CLIFF_CORNER_TOP_LEFT
    {999,    999,     999,    1,   1,     1 }, // CLIFF_CORNER_TOP_RIGHT
    {999,    999,     999,    1,   1,     1 }, // CLIFF_CORNER_BOTTOM_LEFT
    {999,    999,     999,    1,   1,     1 }, // CLIFF_CORNER_BOTTOM_RIGHT
    {  1,      2,       1,    1, 999,   999 }, // CLIFF_INVERSE_CORNER_TOP_LEFT
    {  1,      2,       1,    1, 999,   999 }, // CLIFF_INVERSE_CORNER_TOP_RIGHT
    {  1,      2,       1,    1, 999,   999 }, // CLIFF_INVERSE_CORNER_BOTTOM_LEFT
    {  1,      2,       1,    1, 999,   999 }, // CLIFF_INVERSE_CORNER_BOTTOM_RIGHT
}};

   

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

        std::vector<std::pair<int, int>> calculateMovementRange(Unit& unit);

        int getMoveCost(TileId type, MovementType movementType);

        std::vector<std::pair<int, int>> m_reachableTiles;

        std::vector<std::pair<int, int>> m_attackableTiles;

        
        

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
        bool m_showReachableTiles;
        bool m_showAttackableTiles;

        std::unordered_set<int> m_attackableUnitIds;
};

} // namespace advanced_wars
