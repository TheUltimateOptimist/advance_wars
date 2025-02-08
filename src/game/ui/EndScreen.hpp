#pragma once

#include "../Player.hpp"
#include "../Scene.hpp"

namespace advanced_wars
{

/**
 * End of Game Screen displaying some info and an option to go back to the menu
 */
class Endscreen : public Scene
{
    public:
        /**
         * Constructor
         *
         * @param player The winning player
         */
        Endscreen(Player& player);

        /**
         * Render the scene
         *
         * @param engine An engine object with valid rendering context
         */
        void render(Engine& engine);

        /**
         * Event handler
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

    private:
        SDL_Color   m_color;        // Color for the end-screen (changes based on player)
        int         m_moenyLeft;    // Money still in possession by the winning player
        std::string m_playerString; // The players faction in string form
};

} // namespace advanced_wars
