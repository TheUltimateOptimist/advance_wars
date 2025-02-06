#pragma once
#include "../Scene.hpp"


namespace advanced_wars {

    class HelpMenu : public Scene {

        private: 

        size_t m_selectedOption;
        std::vector<std::pair<std::string, int>> m_options;
        int m_x;
        int m_y;

        public:

        void handleEvent(Engine& engine, SDL_Event& event);

        void update(int x, int y);

        HelpMenu();

        void render(Engine& engine) override;
        

    };

}//namespace advanced_wars