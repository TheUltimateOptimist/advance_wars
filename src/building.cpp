#include "building.hpp"
#include "spritesheet.hpp"

namespace advanced_wars
{

Building::Building(int x, int y, BuildingId id, BuildingFaction faction)
    : x(x), y(y), id(id), faction(faction){};

void Building::render(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.get_spritesheet();

    SDL_Rect src;
    src.x = static_cast<int>(id) * spritesheet->get_building_width();
    src.y = 0;
    src.w = spritesheet->get_building_width();
    src.h = spritesheet->get_building_height();

    SDL_Rect dst;
    dst.x = x * spritesheet->get_tile_width() * scale;
    dst.y = (y - 1) * spritesheet->get_tile_height() * scale;
    dst.w = spritesheet->get_building_width() * scale;
    dst.h = spritesheet->get_building_height() * scale;

    SDL_RenderCopyEx(
        engine.renderer(), spritesheet->get_building_textures()[static_cast<int>(faction)], &src,
        &dst, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars