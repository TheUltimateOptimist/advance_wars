#include "Building.hpp"
#include "Spritesheet.hpp"

namespace advanced_wars
{

Building::Building(int x, int y, BuildingId id, BuildingFaction faction)
    : m_x(x), m_y(y), m_id(id), m_faction(faction) {};

void Building::render(Engine& engine, int scale, int offset_x, int offset_y)
{
    Spritesheet* spritesheet = engine.getSpritesheet();

    SDL_Rect src;
    src.x = static_cast<int>(m_id) * spritesheet->getBuildingWidth();
    src.y = 0;
    src.w = spritesheet->getBuildingWidth();
    src.h = spritesheet->getBuildingHeight();

    SDL_Rect dst;
    dst.x = m_x * spritesheet->getTileWidth() * scale + offset_x;
    dst.y = (m_y - 1) * spritesheet->getTileHeight() * scale + offset_y;
    dst.w = spritesheet->getBuildingWidth() * scale;
    dst.h = spritesheet->getBuildingHeight() * scale;

    SDL_RenderCopyEx(
        engine.renderer(), spritesheet->getBuildingTextures()[static_cast<int>(m_faction)], &src,
        &dst, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars