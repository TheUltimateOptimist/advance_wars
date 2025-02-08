#pragma once
#include "../Scene.hpp"
#include <SDL_ttf.h>

namespace advanced_wars
{

class HelpMenu : public Scene
{

    public:
        void handleEvent(Engine& engine, SDL_Event& event);

        void update(int x, int y);

        HelpMenu();

        void render(Engine& engine) override;

    private:
        size_t                                   m_selectedOption;
        std::vector<std::pair<std::string, int>> m_options;
        int                                      m_x;
        int                                      m_y;

        void renderTextPortion(
            Engine& engine, std::string text, TTF_Font* font, SDL_Color color, int boxWidth,
            int text_x, int text_y);

        void
        renderTexture(SDL_Texture* texture, Engine& engine, int x, int y, int src_x, int src_y);
};

} // namespace advanced_wars