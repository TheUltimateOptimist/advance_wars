#pragma once

#include "../Engine.hpp"
#include "../Unit.hpp"
// #include "../Level.hpp"
#include <SDL_events.h>
#include <string>
#include <unordered_map>

namespace advanced_wars
{

/**
 * Menu to show infos and stats for a Unit
 */
class UnitInfoMenu : public Scene
{
    public:
        /**
         * Constructor
         */
        UnitInfoMenu();

        /**
         * Event Handler
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event) override;

        /**
         * Sets the unit to show stats for
         *
         * @param unit The unit
         */
        void setUnit(Unit& unit);

        /**
         * Renders the menu
         *
         * @param An engine object with valid rendering context
         */
        void render(Engine& engine);

        /**
         * Sets position of menu
         *
         * @param x The new x-position
         * @param y The new y-position
         */
        void update(int x, int y);

    private:
        int   RENDERING_SCALE = 3;
        int   m_x;           // x-position
        int   m_y;           // y-position
        Unit* m_currentUnit; // Current Unit to show stats for
        bool  m_isVisible;   // Visibility toggle

        /**
         * Gets string from MovementType
         *
         * @param type A movementType
         *
         * @return The string describing the movement type
         */
        std::string getMovementTypeString(MovementType type);
};
} // namespace advanced_wars