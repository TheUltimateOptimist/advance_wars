#pragma once
#include "../Scene.hpp"
#include <SDL_ttf.h>

namespace advanced_wars
{

/**
 * A tutorial window to look at the rules of the game
 */
class HelpMenu : public Scene
{

    public:
        /**
         * Event handler
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event);

        void update(int x, int y);

        /**
         * Constructor
         */
        HelpMenu();

        /**
         * Render the scene
         *
         * @param engine An engine object with valid rendering context
         */
        void render(Engine& engine) override;

    private:
        size_t                                   m_selectedOption;
        std::vector<std::pair<std::string, int>> m_options;
        int                                      m_x;
        int                                      m_y;

        /**
         * Renders the given text at the given position in the box
         *
         * @param engine An engine object with valid rendering context
         * @param text The text to render
         * @param font The TTF font to use to render the text
         * @param color The SDL color of the text
         * @param boxWidth Width of the textbox to render into
         * @param text_x x-starting position of the text
         * @param text_y y-starting position of the text
         */
        void renderTextPortion(
            Engine& engine, std::string text, TTF_Font* font, SDL_Color color, int boxWidth,
            int text_x, int text_y);

        /**
         * Renders the given texture at the desired destination
         *
         * @param texture The SDL texture to use for rendering
         * @param engine An engine object with valid rendering context
         * @param x The x-position where to render to
         * @param y The y-position where to render to
         * @param src_x The x-position in the texture
         * @param src_y The y-position in the texture
         */
        void
        renderTexture(SDL_Texture* texture, Engine& engine, int x, int y, int src_x, int src_y);
};

} // namespace advanced_wars