#pragma once

#include "../engine.hpp"
#include "../scene.hpp"
#include <SDL.h>
#include <string>
#include <vector>

namespace advanced_wars
{

class ContextMenu : public Scene
{
    private:
        size_t                   m_selectedOption;
        std::vector<std::string> m_options;
        int                      m_x;
        int                      m_y;

    public:
        ContextMenu();

        void setOptions(const std::vector<std::string>& newOptions);

        void render(Engine* engine) override;

        void handleEvent(SDL_Event& event);

        void update(int x, int y);

        std::string getSelectedOption();

        ~ContextMenu();
};

} // namespace advanced_wars