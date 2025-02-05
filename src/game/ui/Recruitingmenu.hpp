#pragma once

#include "../Scene.hpp"
#include "../Unit.hpp"

namespace advanced_wars
{

    class RecruitingMenu : public Scene {

        private: 

        size_t m_selectedOption;
        std::vector<std::pair<std::string, int>> m_options;
        int m_x;
        int m_y;
        const std::unordered_map <UnitId ,std::pair <std::string, int>> unitNames;
        std::unordered_map<int, UnitId> cost2UnitId;
        UnitId m_selectedId;
    
        void selectSprite();

        public:

        UnitId getSelectedOption();

        void handleEvent(Engine& engine, SDL_Event& event);

        void update(int x, int y);

        RecruitingMenu();

        void setOptions(const std::vector<UnitId> recruitableUnits);

        void render(Engine& engine) override;
        

    };

} //namespace advanced_wars