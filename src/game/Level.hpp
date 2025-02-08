/**
 * Level.hpp
 *
 * @author Frederik Keens
 */

#pragma once

#include "Building.hpp"
#include "Effect.hpp"
#include "Engine.hpp"
#include "Player.hpp"
#include "Scene.hpp"
#include "Tile.hpp"
#include "Unit.hpp"
#include "ui/ContextMenu.hpp"
#include "ui/HelpMenu.hpp"
#include "ui/RecruitingMenu.hpp"
#include "ui/TileMarker.hpp"
#include "ui/UnitInfoMenu.hpp"
#include <SDL.h>
#include <array>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace advanced_wars
{

const int NUM_TILE_IDS = 30;      // update if more tiles are implemented
const int NUM_MOVEMENT_TYPES = 6; // number of different movement types

// Table describing the movement cost of a movement type to get to a given tile
const std::array<std::array<int, NUM_MOVEMENT_TYPES>, NUM_TILE_IDS> moveCostTable = {
    {
     // FOOT, WHEELED, TREAD, AIR, SEA, LANDER
        {1, 2, 1, 1, 999, 999},     // PLAIN
        {999, 999, 999, 1, 1, 1},   // WATER
        {1, 3, 2, 1, 999, 999},     // FOREST
        {2, 999, 999, 1, 999, 999}, // MOUNTAIN
        {1, 1, 1, 1, 999, 999},     // BRIDGE_HORIZONTAL
        {1, 1, 1, 1, 999, 999},     // BRIDE_VERTICAL
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
        /**
         * Constructor
         *
         * @param name String of the level name
         * @param width Width of the level in tiles
         * @param height Height of the level in tiles
         * @param tiles Vector of the levels tiles
         * @param buildings Vector of the levels buildings
         * @param units Vector of the levels starting units
         * @param effects Vector of the levels starting effects
         * @param turnQ Vector of Players that will play this level
         */
        Level(
            std::string name, int width, int height, std::vector<Tile> tiles,
            std::vector<Building> buildings, std::vector<Unit> units, std::vector<Effect> effects,
            std::queue<Player> turnQ);

        /**
         * Load a level from a file.
         *
         * @param path A path to a hdf5 level file
         * @param engine An engine Object with valid unit config
         */
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

        /**
         * Handle events relevant to this scene.
         *
         * @param engine An engine object with valid render context and unit config
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

        /**
         * Add a building to this levels map with a unique id.
         *
         * @param building The building to add
         *
         * @return The unique id of the building added
         */
        int addBuilding(Building building);

        /**
         * Remove a building from this levels map.
         *
         * @param id The unique id of the building to remove
         *
         * @return The building object that was removed
         */
        Building removeBuilding(int id);

        /**
         * Add a unit to this levels map.
         *
         * @param unit The unit to add
         *
         * @return The unique id of the unit added
         */
        int addUnit(Unit unit);

        /**
         * Remove a unit from this levels map.
         *
         * @param id The unique id of the unit to remove
         *
         * @return The unit object that was removed
         */
        Unit removeUnit(int id);

        /**
         * Add an effect to this levels map.
         *
         * @param effect The effect to add
         *
         * @return The unique id of the effect added
         */
        int addEffect(Effect effect);

        /**
         * Remove an effect from this levels map.
         *
         * @param id The unique id of the effect to remove
         *
         * @return The effect object that was removed
         */
        Effect removeEffect(int id);

        /**
         * TODO add comment
         */
        std::vector<std::pair<int, int>> calculateMovementRange(Unit& unit);

        /**
         * Returns the movement cost of a given tile.
         *
         * @param type The tile type to move to
         * @param movementType The movementType of the the unit to move
         *
         * @return The calculated movement cost
         */
        int getMoveCost(TileId type, MovementType movementType);

        /**
         * TODO add comment
         */
        std::vector<std::pair<int, int>> m_reachableTiles;

        /**
         * TODO add comment
         */
        std::vector<std::pair<int, int>> m_attackableTiles;

    private:
        bool                              m_gameOver;  // toggles game end screen
        std::string                       m_name;      // name of the level
        int                               m_width;     // width of the level
        int                               m_height;    // height of the level
        std::vector<Tile>                 m_tiles;     // vector of all floor tiles
        std::unordered_map<int, Building> m_buildings; // map of buildings with unique ids as key
        std::unordered_map<int, Unit>     m_units;     // map of units with unique ids as key
        std::unordered_map<int, Effect>   m_effects;   // map of units with unique ids as key
        std::queue<Player>                m_turnQ; // player turn order, front is currently active

        int            m_selectedUnit;     // unique id of the currently selcted unit
        int            m_selectedBuilding; // unique id of the currently selected building
        int            m_captureBuilding;
        ContextMenu    m_contextMenu;           // A contextMenu when selecting something
        RecruitingMenu m_recruitingMenu;        // Menu when trying to train new units
        bool           toggle_Helpmenu = false; // toggles tutorial screen
        HelpMenu       m_helpMenu;              // The Tutorial screen
        int            m_id;
        LevelState     m_state; // Current level state

        /**
         * TODO subject to remove?
         */
        std::pair<int, int> calcTilePos(int mouseX, int mouseY);

        /**
         * @brief Selects the entity at the given position.
         *
         * Sets m_selectedUnit or m_selectedBuilding of this level
         * to the unique id of the entity at (x,y).
         * Units get preferred if both are on the same position.
         *
         * @param x The tile-based x-position
         * @param y The tile-based y-position
         */
        void selectEntity(int x, int y);

        /**
         * Selects the unit at the given position.
         *
         * @param tileX The x-position
         * @param tileY the y-position
         *
         * @return The unique id of the unit or ```-1``` if none was found
         */
        int selectUnit(int tileX, int tileY);

        /**
         * Selects the building at the given position.
         *
         * @param tileX The x-position
         * @param tileY the y-position
         *
         * @return The unique id of the building or ```-1``` if none was found
         */
        int selectBuilding(int tileX, int tileY);

        TileMarker m_currentPos; // A cursor that marks the current position

        /**
         * Helper function that gets called when in the SELECTING state
         *
         * @param engine An engine object with valid rendering context and config
         * @param event The SDL event to handle
         */
        void handleSelectingEvents(Engine& engine, SDL_Event& event);

        /**
         * Helper function that gets called when in the MENUACTIVE state
         *
         * @param engine An engine object with valid rendering context and config
         * @param event The SDL event to handle
         */
        void handleMenuActiveEvents(Engine& engine, SDL_Event& event);

        /**
         * Helper function that gets called when in the MOVEMENT state
         *
         * @param engine An engine object with valid rendering context and config
         * @param event The SDL event to handle
         */
        void handleMovementEvents(Engine& engine, SDL_Event& event);

        /**
         * Helper function that gets called when in the ATTACKING state
         *
         * @param engine An engine object with valid rendering context and config
         * @param event The SDL event to handle
         */
        void handleAttackingEvents(Engine& engine, SDL_Event& event);

        /**
         * Helper function that gets called when in the RECRUITING state
         *
         * @param engine An engine object with valid rendering context and config
         * @param event The SDL event to handle
         */
        void handleRecruitingEvent(Engine& engine, SDL_Event& event);

        // TODO ClickChecks subject to remove?
        bool clickCheckLeft(int mouseX, int mouseY);
        bool clickCheckRight(int mouseX, int mouseY);

        /**
         * @brief Changes the turn between this levels players.
         *
         * This will call the functions of the players to end the turn and setup the next.
         * After this the front Player in m_turnQ will be the next active player.
         */
        void changeTurn();

        bool m_showReachableTiles;  // Trigger to mark the tiles reachable by movement
        bool m_showAttackableTiles; // Trigger to mark the tiles with valid attack targets

        std::unordered_set<int>
            m_attackableUnitIds; // set of unique unit ids that are valid attack targets

        /**
         * Helper function that gets called when trying to attack a unit
         *
         * @param tilePos (x,y) tile position of the unit to attack
         */
        void handleAttack(std::pair<int, int> tilePos);

        /**
         * Helper function that gets called when trying to move
         *
         * @param tilePos (x,y) tile position of the tile to move to
         */
        void handleMovement(std::pair<int, int> tilePos);

        /**
         * Event Handler for the Cursor
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handlePositionMarker(Engine& engine, SDL_Event& event);

        UnitInfoMenu m_unitInfoMenu;

        bool m_showUnitInfoMenu = false;

        std::pair<int, int> unit_fallback_position;
};

} // namespace advanced_wars
