#include "Building.hpp"
#include "Spritesheet.hpp"
#include <iostream>

namespace advanced_wars
{

Building::Building(int x, int y, BuildingId id, BuildingFaction faction)
    : m_x(x), m_y(y), m_id(id), m_faction(faction) {};

void Building::render(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.getSpritesheet();

    SDL_Rect src;
    src.x = static_cast<int>(m_id) * spritesheet->getBuildingWidth();
    src.y = 0;
    src.w = spritesheet->getBuildingWidth();
    src.h = spritesheet->getBuildingHeight();

    SDL_Rect dst;
    dst.x = m_x * spritesheet->getTileWidth() * scale;
    dst.y = (m_y - 1) * spritesheet->getTileHeight() * scale;
    dst.w = spritesheet->getBuildingWidth() * scale;
    dst.h = spritesheet->getBuildingHeight() * scale;

    SDL_RenderCopyEx(
        engine.renderer(), spritesheet->getBuildingTextures()[static_cast<int>(m_faction)], &src,
        &dst, 0, NULL, SDL_FLIP_NONE);
}

void Building::switch_faction(BuildingFaction faction) {

    this->m_faction = faction;

    if(this->m_id == BuildingId::HEADQUARTER) {
        std::cout << "The game is over!" << std::endl;
    }
}

//implement call to UI to show available units
void Building::on_click() {
    std::cout << "A building is selected!" << std::endl;
};

bool Building::check_spawn(std::unordered_map<int, advanced_wars::Unit>& units) {
    
    for(auto& [id, unit] : units) {
        if(unit.m_x == this->m_x && unit.m_y == this->m_y) {
            return false;
        }
    }

    return true;
}
// can be added as soon as the playerobject is available
bool Building::check_money(int price) {

    if(400 > price) {
        return false;
    }
    return true;

}

} // namespace advanced_wars