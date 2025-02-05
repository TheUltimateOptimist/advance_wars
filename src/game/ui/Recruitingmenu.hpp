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
 
        void handleEvent(Engine& engine, SDL_Event& event);

        

        void selectSprite();

        std::string getSelectedOption();

        std::string unitIdToString(UnitId id);

        public:

        void update(int x, int y);

        RecruitingMenu();

        void setOptions(const std::vector<UnitId> recruitableUnits);

        void render(Engine& engine) override;
        

    };

} //namespace advanced_wars