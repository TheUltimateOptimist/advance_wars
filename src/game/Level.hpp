#pragma once

#include "Building.hpp"
#include "Effect.hpp"
#include "Engine.hpp"
#include "Player.hpp"
#include "Scene.hpp"
#include "Tile.hpp"
#include "Unit.hpp"
#include "ui/Contextmenu.hpp"
#include "ui/Recruitingmenu.hpp"
#include "ui/TileMarker.hpp"
#include "ui/Helpmenu.hpp"
#include <SDL.h>
#include <array>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace advanced_wars
{

const int NUM_TILE_IDS = 30; // Aktualisieren, falls weitere IDs hinzugefügt werden
const int NUM_MOVEMENT_TYPES = 6;

const std::array<std::array<int, NUM_MOVEMENT_TYPES>, NUM_TILE_IDS> moveCostTable = {
    {
     // FOOT, WHEELED, TREAD, AIR, SEA, LANDER
        {1, 2, 1, 1, 999, 999},     // PLAIN
        {999, 999, 999, 1, 1, 1},   // WATER
        {1, 3, 2, 1, 999, 999},     // FOREST
        {2, 999, 999, 1, 999, 999}, // MOUNTAIN
        {1, 1, 1, 1, 999, 999},     // BRIDGE_HORIZONTAL
        {1, 1, 1, 1, 999, 999},     // STREET_HORIZONTAL
        {1, 1, 1, 1, 999, 999},     // STREET_VERTICAL
        {1, 1, 1, 1, 999, 999},     // STREET_CROSSING
        {1, 1, 1, 1, 999, 999},     // STREET_JUNCTION_RIGHT
        {1, 1, 1, 1, 999, 999},     // STREET_JUNCTION_LEFT
        {1, 1, 1, 1, 999, 999},     // STREET_JUNCTION_DOWN
        {1, 1, 1, 1, 999, 999},     // STREET_JUNCTION_UP
        {1, 1, 1, 1, 999, 999},     // STREET_CORNER_TOP_LEFT
        {1, 1, 1, 1, 999, 999},     // STREET_CORNER_TOP_RIGHT
        {1, 1, 1, 1, 999, 999},     // STREET_CORNER_BOTTOM_LEFT
        {1, 1, 1, 1, 999, 999},     // STREET_CORNER_BOTTOM_RIGHT
        {999, 999, 999, 1, 2, 2},   // RIFF
        {999, 999, 999, 1, 1, 1},   // CLIFF_TOP
        {999, 999, 999, 1, 1, 1},   // CLIFF_BOTTOM
        {999, 999, 999, 1, 1, 1},   // CLIFF_LEFT
        {999, 999, 999, 1, 1, 1},   // CLIFF_RIGHT
        {999, 999, 999, 1, 1, 1},   // CLIFF_CORNER_TOP_LEFT
        {999, 999, 999, 1, 1, 1},   // CLIFF_CORNER_TOP_RIGHT
        {999, 999, 999, 1, 1, 1},   // CLIFF_CORNER_BOTTOM_LEFT
        {999, 999, 999, 1, 1, 1},   // CLIFF_CORNER_BOTTOM_RIGHT
        {999, 999, 999, 1, 1, 1},   // CLIFF_INVERSE_CORNER_TOP_LEFT
        {999, 999, 999, 1, 1, 1},   // CLIFF_INVERSE_CORNER_TOP_RIGHT
        {999, 999, 999, 1, 1, 1},   // CLIFF_INVERSE_CORNER_BOTTOM_LEFT
        {999, 999, 999, 1, 1, 1},   // CLIFF_INVERSE_CORNER_BOTTOM_RIGHT
    }
};

enum class LevelState
{
    SELECTING_STATE,
    MOVEMENT_STATE,
    ANIMATING_STATE,
    MENUACTIVE_STATE,
    ATTACKING_STATE,
    RECRUITING_STATE,
};

/**
 * @brief The main window of the game
 */
class Level : public Scene
{
    public:
        Level(
            std::string name, int width, int height, std::vector<Tile> tiles,
            std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects,
            std::queue<Player> turnQ);

        static std::shared_ptr<Level> loadLevel(const std::string& path, Engine& engine);

        void render(Engine& engine);

        /*
        on event
            key down
                escape      ->  deselect/ open pause menu
                key left    ->  move one tile left
                key right   ->  move one tile right
                key up      ->  move one tile up / change context menu selection up
                key down    ->  move one tile down / change context menu selection down
                key enter   ->  confirm selection in context menu /
                                confirm selected position(moving)/
                                select entity on tile
            mousebutton down
                button left ->  select field/building/unit/
                                move to position
        */
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
        std::string                       m_name;
        int                               m_width;
        int                               m_height;
        std::vector<Tile>                 m_tiles;
        std::unordered_map<int, Building> m_buildings;
        std::unordered_map<int, Unit>     m_units;
        std::unordered_map<int, Effect>   m_effects;
        std::queue<Player>                m_turnQ;

        int            m_selectedUnit;
        int            m_selectedBuilding;
        int            m_captureBuilding;
        ContextMenu    m_contextMenu;
        RecruitingMenu m_recruitingMenu;
        bool toggle_Helpmenu = false;
        HelpMenu       m_helpMenu;
        int            m_id;
        LevelState     m_state;

        std::pair<int, int> calcTilePos(int mouseX, int mouseY);
        void                selectEntity(int x, int y);
        int                 selectUnit(int tileX, int tileY);
        int                 selectBuilding(int tileX, int tileY);

        TileMarker m_currentPos;

        void handleSelectingEvents(Engine& engine, SDL_Event& event);
        void handleMenuActiveEvents(Engine& engine, SDL_Event& event);
        void handleMovementEvents(Engine& engine, SDL_Event& event);
        void handleAttackingEvents(Engine& engine, SDL_Event& event);
        void handleRecruitingEvent(Engine& engine, SDL_Event& event);

        bool clickCheckLeft(int mouseX, int mouseY);
        bool clickCheckRight(int mouseX, int mouseY);

        void changeTurn();

        bool m_showReachableTiles;
        bool m_showAttackableTiles;

        std::unordered_set<int> m_attackableUnitIds;

        void handleAttack(std::pair<int, int> tilePos);
        void handleMovement(std::pair<int, int> tilePos);
        void handlePositionMarker(Engine& engine, SDL_Event& event);

        std::pair<int, int> unit_fallback_position;

};

} // namespace advanced_wars
