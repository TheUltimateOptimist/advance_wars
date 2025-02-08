/**
 * Recuitingmenu.hpp
 *
 * @author
 */

#pragma once

#include "../Scene.hpp"
#include "../Unit.hpp"

namespace advanced_wars
{

class RecruitingMenu : public Scene
{

    private:
        size_t                                   m_selectedOption; // Currently selected option
        std::vector<std::pair<std::string, int>> m_options;        // vector of all options
        int                                      m_x;              // x-position
        int                                      m_y;              // y-position

        /* map unit type to a pair of its name and cost */
        const std::unordered_map<UnitId, std::pair<std::string, int>> unitNames;
        std::unordered_map<int, UnitId> cost2UnitId;  // maps unit cost to id
        UnitId                          m_selectedId; // Currenly selected unitId

    public:
        /**
         * Constructor
         */
        RecruitingMenu();

        /**
         * Gets the currently selected unit.
         *
         * @return The unitId
         */
        UnitId getSelectedOption();

        /**
         * Event handler for the menu.
         *
         * @param engine An engine object with a valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

        /**
         * Update the position of the menu.
         *
         * @param x The new x-position
         * @param y The new y-position
         */
        void update(int x, int y);

        /**
         * Sets this menus options.
         *
         * @param recruitableUnits Vector of all units to be recruitable by this menu
         */
        void setOptions(const std::vector<UnitId> recruitableUnits);

        /**
         * Render this menu.
         *
         * @param engien An engine object with a valid rendering context
         */
        void render(Engine& engine) override;
};

} // namespace advanced_wars