#pragma once

#include "../Scene.hpp"

namespace advanced_wars
{

    class Recruitingmenu : public Scene {

        private: 
        size_t m_selectedOption;
        std::vector<std::string> m_options;
        int m_x;
        int m_y;

        void setOptions(const std::vector<std::string>& newOptions);

        void render(Engine& engine) override;

        void handleEvent(Engine& engine, SDL_Event& event);

    };

} //namespace advanced_wars