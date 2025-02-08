/**
 * Contextmenu.hpp
 *
 * @author
 */

#pragma once

#include "../Engine.hpp"
#include "../Scene.hpp"
#include <SDL.h>
#include <string>
#include <vector>

namespace advanced_wars
{

/**
 * A context menu in the game
 */
class ContextMenu : public Scene
{
    private:
        std::vector<std::string> m_options;        // The menu options
        size_t                   m_selectedOption; // The currently selected option

        int m_x; // x-position
        int m_y; // y-position

    public:
        /**
         * Constructor
         */
        ContextMenu();

        /**
         * Sets this menus options.
         *
         * @param A string vector containing the options
         */
        void setOptions(const std::vector<std::string>& newOptions);

        /**
         * Render the menu.
         *
         * @param engine An engine object with valid rendering context
         */
        void render(Engine& engine) override;

        /**
         * Event handler for the menu.
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

        /**
         * Sets this menus new position.
         *
         * @param x The new x-position
         * @param y The new y-position
         */
        void update(int x, int y);

        /**
         * Gets the currently selected option
         *
         * @return The option as string
         */
        std::string getSelectedOption();

        /**
         * Destructor
         */
        ~ContextMenu();
};

} // namespace advanced_wars