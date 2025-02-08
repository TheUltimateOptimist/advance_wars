#pragma once

#include "../Engine.hpp"
#include "../Unit.hpp"
//#include "../Level.hpp"
#include <SDL_events.h>
#include <string>
#include <unordered_map>

namespace advanced_wars {

class UnitInfoMenu  : public Scene{
public:
    UnitInfoMenu();
    void handleEvent(Engine& engine, SDL_Event& event) override;
    
    void setUnit(Unit& unit);
    void render(Engine& engine);
    void update(int x, int y);

    

    
private:
    int RENDERING_SCALE = 3;
    int m_x;
    int m_y;
    Unit* m_currentUnit;
    bool m_isVisible;
    
    std::string getMovementTypeString(MovementType type);
};
}