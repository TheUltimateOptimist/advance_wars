#pragma once

#include "../Scene.hpp"

namespace advanced_wars
{

    class RecruitingMenu : public Scene {

        private: 

        size_t m_selectedOption;
        std::vector<std::string> m_options;
        int m_x;
        int m_y;

        std::vector<std::vector<std::vector<std::pair<SDL_Texture *, int>>>> renderableunit;

        

        void handleEvent(Engine& engine, SDL_Event& event);

        

        void selectSprite();

        std::string getSelectedOption();

        public:

        void update(int x, int y);

        RecruitingMenu();

        void setOptions(const std::vector<std::string>& newOptions);

        void render(Engine& engine) override;
        

    };

} //namespace advanced_wars