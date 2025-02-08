#pragma once

#include "../Scene.hpp"
#include "../Unit.hpp"

namespace advanced_wars
{

class RecruitingMenu : public Scene
{

    public:
        /**
         * Returns the selected Unit
         *
         * @return The currently selected UnitId
         */
        UnitId getSelectedOption();

        /**
         * Event handler
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

        /**
         * Adjusts the position of the menu
         */
        void update(int x, int y);

        /**
         * Constructor
         */
        RecruitingMenu();

        /**
         * Sets the menu options
         *
         * @param recruitableUnits A vector of recuitable units to show
         */
        void setOptions(const std::vector<UnitId> recruitableUnits);

        /**
         * Render the menu
         *
         * @param engine An engine object with valid rendering context
         */
        void render(Engine& engine) override;

    private:
        size_t                                        m_selectedOption; // Currently selected option
        std::vector<UnitId>                           m_options;        // All available options
        int                                           m_x;              // x-position
        int                                           m_y;              // y-position
        const std::unordered_map<UnitId, std::string> unitNames;    // maps UnitIds to unnits name
        std::unordered_map<int, UnitId>               cost2UnitId;  // maps cost to unitIds
        UnitId                                        m_selectedId; // Currently selected Unit

        void selectSprite();
};

} // namespace advanced_wars