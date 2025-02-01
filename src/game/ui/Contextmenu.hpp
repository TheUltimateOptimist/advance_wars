#pragma once

#include "../Engine.hpp"
#include "../Scene.hpp"
#include <SDL.h>
#include <string>
#include <vector>

namespace advanced_wars
{

class ContextMenu : public Scene
{
    private:
        std::vector<std::string> m_options;
        size_t                   m_selectedOption;

        int m_x;
        int m_y;

    public:
        ContextMenu();

        void setOptions(const std::vector<std::string>& newOptions);

        void render(Engine& engine) override;

        void handleEvent(Engine& engine, SDL_Event& event);

        void update(int x, int y);

        std::string getSelectedOption();

        ~ContextMenu();
};

} // namespace advanced_wars